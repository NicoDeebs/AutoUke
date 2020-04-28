/*
* Written by Nicholas DiBattista and Ali El Hakie
* ECE 4180 Final project
* 4/24/2020
*/
#include "mbed.h"
#include "SDFileSystem.h"
#include "Solenoids.h"
#include "Metronome.h"
#include "Decode.h"


//Program Constants
#define arrSize 300 //Preallocated size of the chord array
#define minBPM 25.0 //BPM setting when potentiometer is maximum
#define maxBPM 200.0 //BPM setting when potentiometer is minimum
#define turnOffLimit .99 //If the potentiometer reading is greater than this, the device turns off.
#define turnOnLimit .97 //If the potentiometer reading is less than this, the device turns on.
#define OffStateWaitPeriod 0.2 //This is how long the program takes to check to see if the device has been turned on.

#define peri(a) (15.0/a) //Converts beats per minute into a ~quarter period~ in seconds
#define waitPeriod peri(maxBPM) + pot*(peri(minBPM)-peri(maxBPM)) //Formula for calculating the bpm from the pot


 
//Program Variables 
SDFileSystem sd(p5, p6, p7, p8, "sd"); //SD card
BusOut String1(sol1,sol5,sol9,sol3); //Leftmost string on the ukelele
BusOut String2(sol3,sol7,sol11,sol15); //Second string from the left
BusOut String3(sol4,sol8,sol12,sol16); //Second string from the right
BusOut String4(sol2,sol6,sol10,sol14); //Rightmost string on the ukelele
int chordArray[arrSize]; //Chord array, preallocated to arrSize, a program constant
int chordCurrentIndex; //Current chord
int chordMaxIndex; //Size of the chord array, defined durning runtime by the number of valid chords found in the text file.
Metronome metronome(LED1,LED2,LED3,LED4,FourFour); //Instance of a custom class
AnalogIn pot(p16); //Potentiometer reading

/*
* Calls decoder on the character given by c.
* If it returns zero, does nothing.
* If it returns a chord, adds it to the chord array.
*/
void chordCall(char c) {
    int chord = readChar(c)
    if(chord) { 
        if(chordMaxIndex<arrSize) chordArray[chordMaxIndex++] = chord;
    }
}

/*
* Decodes a file named "tab.txt".
* Utilizes the state machine in decoder.h
*/
void readSD() {
    FILE *fp = fopen("/sd/tab.txt", "r");
    if(!fp) return;
    char ch = getc(fp);
    if((ch>='0')&(ch<='4')) { //If the character is a numeral between 0 and 4, sets the time signature
        metronome.setTime((int)(ch -'0'));
        if((ch = getc(fp))==',') ch = getc(fp); //Throws away the delimiting comma if there is one
    }
    while(ch-255) {
        chordCall(ch);
        ch = getc(fp);
    }
    chordCall(',');
    fclose(fp);
}
/*
 * Performs the "Inverse" of the chord compressing function in
 * Decoder.h. Converts the number output from that into
 * outputs on the mbed.
*/
void sendChord(int chord) {
    String1 = 0b1<<(((chord>>9)&(0b111))-1);
    String2 = 0b1<<(((chord>>6)&(0b111))-1);
    String3 = 0b1<<(((chord>>3)&(0b111))-1);
    String4 = 0b1<<(((chord)&(0b111))-1);
}
/*
 * Execution ends up here while the device is turned off.
 * Resets program state
 * Checks if the potentiometer goes above threshold, if it does, restarts and returns.
*/
void offRoutine() {
    sendChord(0);
    metronome.reset();
    chordMaxIndex = 0;
    chordCurrentIndex = 0;
    while(pot > turnOnLimit) wait(OffStateWaitPeriod);
    readSD();
}
/*
 * Included for debug purposes. Creates a log file that dumps the current chord array.
 * Mainly used to debug decode.h. Unused in build.
*/
void log() {
    FILE *fp = fopen("/sd/log.txt", "w");
    if(!fp) return;
    int chord;
    int i = 0;
    for(i=0;i<chordMaxIndex;i++) {
        chord = chordArray[i];
        fprintf(fp,"%d,%d,%d,%d; ",(((chord>>9)&(0b111))),(((chord>>6)&(0b111))),(((chord>>3)&(0b111))),(((chord)&(0b111))));
    }
    fclose(fp);
}

int main() {
    readSD();
    //log();
    while(1) {
        if(metronome.clockTick()) {
            sendChord(chordArray[chordCurrentIndex++]);
            if(chordCurrentIndex==chordMaxIndex)chordCurrentIndex = 0;
        }
        if(pot > turnOffLimit) offRoutine();
        wait(waitPeriod);
    }
}
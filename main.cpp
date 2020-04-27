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
#define arrSize 300 //Maximum size of the chord array
#define minBPM 25.0 //BPM setting when potentiometer is maximum
#define maxBPM 200.0 //BPM setting when potentiometer is minimum
#define turnOffLimit .98

#define peri(a) (15.0/a)
#define diff (peri(minBPM)-peri(maxBPM))


 
//Program Variables 
SDFileSystem sd(p5, p6, p7, p8, "sd"); //SD card
BusOut String1(sol1,sol5,sol9,sol3); //Leftmost string on the ukelele
BusOut String2(sol3,sol7,sol11,sol15);
BusOut String3(sol4,sol8,sol12,sol16);
BusOut String4(sol2,sol6,sol10,sol14); //Rightmost string on the ukelele
int chordArray[arrSize]; //Chord array, preallocated to arrSize
int chordCurrentIndex; //Current chord
int chordMaxIndex; //Size of the chord array
Metronome metronome(LED1,LED2,LED3,LED4,FourFour); //Instance of a custom class
AnalogIn pot(p16); //Potentiometer reading


/*
* Decodes a file named "tab.txt".
* Utilizes the state machine in decoder.h
*/
void readSD() {
    FILE *fp = fopen("/sd/tab.txt", "r");
    if(!fp) return;
    char ch = getc(fp);
    if((ch>='0')&(ch<='4')) {
        metronome.setTime((int)(ch -'0'));
        getc(fp);
        ch = getc(fp);
    }
    int chord;
    while(ch-255) {
        if(chord = readChar(ch)) { //Intentional assignment inside conditional
            if(chordMaxIndex<arrSize) chordArray[chordMaxIndex++] = chord;
        }
        ch = getc(fp);
    }
    if(chord = readChar(',')) { //This basically "appends" a delimiting character to the file
        if(chordMaxIndex<arrSize) chordArray[chordMaxIndex++] = chord;
    }
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

void offRoutine() {
    sendChord(0);
    metronome.reset();
    chordMaxIndex = 0;
    chordCurrentIndex = 0;
    while(pot > turnOffLimit) wait(.5);
    readSD();
}

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
    log();
    while(1) {
        if(metronome.clockTick()) {
            sendChord(chordArray[chordCurrentIndex++]);
            if(chordCurrentIndex==chordMaxIndex)chordCurrentIndex = 0;
        }
        if(pot > turnOffLimit) offRoutine();
        wait(peri(maxBPM) + pot*diff);
    }
}
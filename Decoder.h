#include "mbed.h"
//EXCLUDED FROM FINAL BUILD
//Replaced with decode.h

enum notes {A=0,As=1,Bb=1,B=2,C=3,Cs=4,Db=4,D=5,Ds=6,Eb=6,E=7,F=8,Fs=9,Gb=9,G=10,Gs=11,Ab=11}; //Enum type for keyboard notes. Identical notes share an enum value. (I. E. Ab = B#), value corresponds to half step distance from A.
enum chordType {MAJ,MIN,AUG,DIM,DOM7,MIN7,MAJ7,MAJ6,MIN6,ADD9,MIN9,MAJ9,SUS2,SUS4,SEVSUS4}; //Enum type for chord types, A note and type can together uniquely characterize a chord

int letarray[7] = {0,2,3,5,7,8,10};//Used to convert an ASCII character into a note (For example, ascii(G-A) is 7, but G natural corresponds to note 10 in the ENUM type

#define ch(a,b,c,d) ((a<<9)|(b<<6)|(c<<3)|d) //chord(string 1, string 2, string 3, string 4), each string takes a number that represents what fret each string is for that chord. 3 bit number, 0-5

const int frets[12][15] = {
//    |---MAJ---| |---MIN---| |---AUG---| |---DIM---| |---DOM7--| |---MIN7--| |---MAJ7--| |---MAJ6--| |---MIN6--| |---ADD9--| |---MIN9--| |---MAJ9---| |---SUS2--| |---SUS4--| |--7SUS4--| 
/*A♮*/{ch(2,1,0,0),ch(2,0,0,0),ch(2,1,1,4),ch(2,3,2,3),ch(0,1,0,0),ch(0,0,0,0),ch(1,1,0,0),ch(2,1,2,0),ch(2,0,2,0),ch(2,1,0,1),ch(2,0,0,2),ch(0,1,0,2),ch(2,4,5,2),ch(2,2,0,0),ch(0,2,0,0)},
/*A#*/{ch(3,2,1,1),ch(3,1,1,1),ch(3,2,2,1),ch(3,1,0,1),ch(1,2,1,1),ch(1,1,1,1),ch(3,2,1,0),ch(0,2,1,1),ch(3,1,3,1),ch(3,2,1,3),ch(3,1,1,3),ch(3,2,4,3),ch(3,0,1,1),ch(3,3,1,1),ch(1,3,1,1)},
/*B♮*/{ch(4,3,2,2),ch(4,2,2,2),ch(4,3,3,2),ch(4,2,1,2),ch(2,3,2,2),ch(2,2,2,2),ch(4,3,2,1),ch(1,3,2,2),ch(1,2,2,2),ch(4,3,2,4),ch(2,1,0,2),ch(4,3,5,4),ch(4,1,2,2),ch(4,4,2,2),ch(2,4,2,2)},
/*C♮*/{ch(0,0,0,3),ch(0,3,3,3),ch(1,0,0,3),ch(2,3,2,3),ch(0,0,0,1),ch(3,3,3,3),ch(0,0,0,2),ch(0,0,0,0),ch(2,3,3,3),ch(0,4,3,3),ch(5,3,3,5),ch(0,2,0,1),ch(0,2,3,3),ch(0,0,1,3),ch(0,0,1,1)},
/*C#*/{ch(1,1,1,4),ch(1,4,4,4),ch(2,1,1,0),ch(0,2,0,4),ch(1,1,1,2),ch(1,1,0,2),ch(1,1,0,2),ch(1,1,1,3),ch(1,1,1,1),ch(1,1,0,1),ch(1,3,1,4),ch(1,3,1,2),ch(1,3,4,4),ch(1,1,2,4),ch(1,1,2,2)},
/*D♮*/{ch(2,2,2,1),ch(2,2,1,0),ch(3,2,2,1),ch(1,2,1,4),ch(2,2,2,3),ch(2,2,1,3),ch(2,2,2,4),ch(2,2,2,2),ch(2,2,1,2),ch(2,4,2,5),ch(2,5,0,5),ch(2,4,2,3),ch(2,2,0,0),ch(0,2,3,0),ch(2,2,3,3)},
/*D#*/{ch(0,3,3,1),ch(3,3,2,1),ch(0,3,3,2),ch(2,3,2,0),ch(3,3,3,4),ch(3,3,2,4),ch(3,3,3,5),ch(3,3,3,3),ch(3,3,2,3),ch(0,3,1,1),ch(3,0,5,6),ch(0,1,1,1),ch(3,3,4,4),ch(1,3,4,1),ch(3,3,4,4)},
/*E♮*/{ch(1,4,0,2),ch(0,4,3,2),ch(1,0,0,3),ch(0,1,0,1),ch(1,2,0,2),ch(0,2,0,2),ch(1,3,0,2),ch(4,4,4,4),ch(0,1,0,2),ch(1,4,2,2),ch(0,4,2,2),ch(1,2,2,2),ch(4,4,2,2),ch(2,4,5,2),ch(4,4,5,5)},
/*F♮*/{ch(2,0,1,0),ch(1,0,1,3),ch(2,1,1,0),ch(1,2,1,2),ch(2,3,1,3),ch(1,3,1,3),ch(2,4,1,3),ch(2,2,1,3),ch(1,2,1,3),ch(0,0,1,0),ch(0,5,4,3),ch(2,3,3,3),ch(0,0,1,3),ch(3,0,1,1),ch(5,5,6,6)},
/*F#*/{ch(3,1,2,1),ch(2,1,2,0),ch(3,2,2,1),ch(2,3,2,3),ch(3,4,2,4),ch(2,4,2,4),ch(3,5,2,4),ch(3,3,2,4),ch(2,1,2,3),ch(1,1,2,1),ch(1,2,2,0),ch(1,1,0,1),ch(1,1,2,4),ch(4,1,2,2),ch(6,6,7,7)},
/*G♮*/{ch(0,2,3,2),ch(0,2,3,1),ch(0,3,3,2),ch(0,1,3,1),ch(0,2,1,2),ch(0,2,1,1),ch(0,2,2,2),ch(0,2,0,2),ch(0,2,0,1),ch(0,2,5,2),ch(2,2,3,1),ch(2,2,1,2),ch(0,2,3,0),ch(0,2,3,3),ch(0,2,1,3)},
/*G#*/{ch(4,2,3,2),ch(4,3,4,2),ch(1,0,0,3),ch(1,2,1,2),ch(1,3,2,3),ch(1,3,2,2),ch(1,3,3,3),ch(1,3,1,3),ch(3,4,3,5),ch(3,3,4,3),ch(3,3,4,2),ch(3,3,2,3),ch(1,3,4,1),ch(1,3,4,4),ch(1,3,2,4)}};


enum states {BASE,READ_NOTE,CHORDS_M,CHORDS_A,CHORDS_D,CHORDS_S,CHORDS_6,CHORDS_7,CHORDS_9, //Basic 1 letter chord states
CHORDS_M7,CHORDS_MA,CHORDS_M9,CHORDS_M6,CHORDS_MAJ,CHORDS_MAJ7, //Chord states starting with the letter M
CHORDS_AU,CHORDS_AUG,CHORDS_AD,CHORDS_ADD,CHORDS_ADD9, //Chord states starting with the letter A
CHORDS_DI,CHORDS_DIM,CHORDS_SU,CHORDS_SUS,CHORDS_SUS2,CHORDS_SUS4, //Chord states starting with D or S
CHORDS_7S,CHORDS_7SU,CHORDS_7SUS,CHORDS_7SUS4}; //Chord states starting with 7

const char names[6] = "\0\0\0\0\0";
//const char names[15][5] = {"\0

int state = BASE;
int note = -1;
int chord = -1;

/*
* State-machine like interpreting system for the SD card file.
* Accepts a character, then updates the state.
* Returns 0 if a full chord has not been input, otherwise returns the
* chord that was input by indexing the frets array.
*/
int readChar(char inChar) {
        if((inChar>='a') & (inChar<='z')) inChar = inChar - 'a' + 'A';//Code converts lower case letters to upper case, as case sensativity isn't important for this state machine
        if(inChar==' ') return 0; //Ignore spaces
        switch(state) {
            case BASE:
                note = -1;
                chord = -1;
                if((inChar>='A') & (inChar<='G')) { //Alphabetical inputs in base state are interpreted as the start of a chord declaration
                    note = letarray[(int)(inChar-'A')];
                    state = READ_NOTE;
                } else {
                    state = BASE;
                }
            break;
            case READ_NOTE:
                if(inChar=='#') { //Sharp sign adds one to the note
                    note = (note + 1)%12; //Stays within positive octave of A
                    state = READ_NOTE; //Stays in same state to support multiple sharps
                } else if(inChar=='B') {//Flat sign (b is always upper) subtracts one from the note
                    note = (note- 1)%12; //Stays within positive octave of A
                    state = READ_NOTE;//Stays in same state to support multiple flats
                } else if(inChar==',') {
                    chord = MAJ;
                    state = BASE;
                    return frets[note][chord];
                } else if(inChar=='M') {
                    state = CHORDS_M;
                } else if(inChar=='A') {
                    state = CHORDS_A;
                } else if(inChar=='D') {
                    state = CHORDS_D;               
                } else if(inChar=='S') {
                    state = CHORDS_S;
                } else if(inChar=='6') {
                    state = CHORDS_6;
                } else if(inChar=='7') {
                    state = CHORDS_7;
                } else if(inChar=='9') {
                    state = CHORDS_9;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_M: 
                if(inChar==',') {
                    chord = MIN;
                    state = BASE;
                    return frets[note][chord];
                } else if(inChar=='7') {
                    state = CHORDS_M7;
                } else if(inChar=='A') {
                    state = CHORDS_MA;
                } else if(inChar=='6') {
                    state = CHORDS_M6;//Look at this janky code
                } else if(inChar=='9') {
                    state = CHORDS_M9;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_A:
                if(inChar=='U') {
                    state = CHORDS_AU;
                } else if(inChar=='D') {
                    state = CHORDS_AD;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_D:
                if(inChar=='I') {
                    state = CHORDS_DI;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_S:
                if(inChar=='U') {
                    state = CHORDS_SU;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_6:
                if(inChar==',') {
                    chord = MAJ6;
                    state = BASE;
                    return frets[note][chord];
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_7:
                if(inChar=='S') {
                    state = CHORDS_7S;
                } else if(inChar==',') {
                    chord = DOM7;
                    state = BASE;//It's awful isn't it
                    return frets[note][chord];
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_9:
                if(inChar==',') {
                    chord = MAJ9;
                    state = BASE;
                    return frets[note][chord];
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_M7:
                if(inChar==',') {
                    chord = MIN7;
                    state = BASE;
                    return frets[note][chord];
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_MA:
                if(inChar=='J') {
                    state = CHORDS_MAJ;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_M9:
                if(inChar==',') {
                    chord = MIN7;
                    state = BASE;
                    return frets[note][chord];
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_M6:
                if(inChar==',') {
                    chord = MIN6;
                    state = BASE;//Tastes like spaghetti
                    return frets[note][chord];
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_MAJ:
                if(inChar=='7') {
                    state = CHORDS_MAJ7;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_MAJ7:
                if(inChar==',') {
                    chord = MAJ7;
                    state = BASE;
                    return frets[note][chord];
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_AU:
                if(inChar=='G') {
                    state = CHORDS_AUG;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_AD:
                if(inChar=='D') {
                    state = CHORDS_ADD;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_AUG:
                if(inChar==',') {
                    chord = AUG;
                    state = BASE;
                    return frets[note][chord];
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_ADD:
                if(inChar=='9') {
                    state = CHORDS_ADD9;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_ADD9:
                if(inChar==',') {
                    chord = ADD9;
                    state = BASE;
                    return frets[note][chord];
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_DI:
                if(inChar=='M') {
                    state = CHORDS_DIM;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_DIM:
                if(inChar==',') {
                    chord = DIM;
                    state = BASE;
                    return frets[note][chord];
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_SU:
                if(inChar=='S') {
                    state = CHORDS_DIM;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_SUS:
                if(inChar=='2') {
                    state = CHORDS_SUS2;
                } else if(inChar=='4') {
                    state = CHORDS_SUS4;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_SUS2:
                if(inChar==',') {
                    chord = SUS2;
                    state = BASE;
                    return frets[note][chord];
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_SUS4:
                if(inChar==',') {
                    chord = SUS4;
                    state = BASE;
                    return frets[note][chord];
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_7S:
                if(inChar=='U') {
                    state = CHORDS_7SU;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_7SU:
                if(inChar=='S') {
                    state = CHORDS_7SUS;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_7SUS:
                if(inChar=='4') {
                    state = CHORDS_7SUS4;
                } else {
                    state = BASE;
                }
            break;
            case CHORDS_7SUS4:
                if(inChar==',') {
                    chord = SEVSUS4;
                    state = BASE;
                    return frets[note][chord];
                } else {
                    state = BASE;
                }
            break;
        }
        return 0;
    }
    
    
    

    

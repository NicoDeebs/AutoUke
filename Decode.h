#define ch(a,b,c,d) ((a<<9)|(b<<6)|(c<<3)|d|(1<<31)) //chord(string 1, string 2, string 3, string 4), each string takes a number that represents what fret each string is for that chord. 3 bit number, 0-5
#define numChords 15

static const int frets[12][numChords] = {
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
static const int letarray[7] = {0,2,3,5,7,8,10};//Used to convert an ASCII character into a note (For example, ascii(G-A) is 7, but G natural corresponds to note 10 in the ENUM type
static const char names[numChords][6] = {"\0\0\0\0\0","M\0\0\0\0","AUG\0\0","DIM\0\0","7\0\0\0\0","M7\0\0\0","MAJ7\0","MAJ6\0","M6\0\0\0","ADD9\0","M9\0\0\0","MAJ9\0","SUS2\0","SUS4\0","7SUS4"}; //Hotstrings to be checked for in tab.txt

static char inputName[5];
static int state;
static int note;

/*
Compares inputName vector to names[nameInd], returns 1 if they are equal and 0 otherwise
*/
int compareIn(int nameInd) {
    int i;
    for(i = 0; i<5;i++) {
        if(!(inputName[i]==names[nameInd][i])) return 0;
    }
    return 1;
}
/*
Sets inputName to a vector of zeros
*/
void resetInput() {
    int i;
    for(i = 0; i<5;i++) {
        inputName[i]='\0';
    }
}

/*
* State-machine like interpreting system for the SD card file.
* Accepts a character, then updates the state.
* Returns 0 if a full chord has not been input, otherwise returns the
* chord that was input by indexing the frets array.
*/
int readChar(char inChar) {
        if((inChar>='a') & (inChar<='z')) inChar = inChar - 'a' + 'A';//Code converts lower case letters to upper case, as case sensativity isn't important for this state machine
        if(inChar==' ') return 0; //Ignore spaces
        if(state==0) {
            note = -1;
            resetInput();
            if((inChar>='A') & (inChar<='G')) { //Alphabetical inputs in base state are interpreted as the start of a chord declaration
                note = letarray[(int)(inChar-'A')]; //Converts characters A through G into an integer number of half steps from A.
                state++;
            }
        } else if((state==1)&&(inChar=='#')) {
            note = (note + 1)%12; //If sharp, adds 1 to note and mods to stay within positive octave(0-11) of A.
        } else if((state==1)&&(inChar=='B')) {
            note = (note - 1)%12; //If flat, subtracts 1 from note and mods to stay within positive octave(0-11) of A.
        }else if(state > 6){ 
            state = 0; //If 6 chars have been entered without a comma, resets to state 0
        } else if(inChar==',') { //Checks letters entered if a comma has been entered
            state = 0; 
            int i;
            for(i = 0; i < numChords;i++) {
                if(compareIn(i)) return frets[note][i]; //Checks if a valid chord was entered, and if so, returns that chord's string numbers
            }
        } else {
            state++;
        }
        return 0; //If an entire chord has not yet been entered, returns zero.
}
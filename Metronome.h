#include "mbed.h"
//Class to make a metronome using 4 led's.

enum timeSig {FourFour,ThreeFour,SixEight,Eighths,Sixteenths};

#define blinkSpeed 0.25
#define blinkCount 4

const int order[5][8] = {
{0b0001,0b0010,0b0100,0b1000,0b0000,0b0000,0b0000,0b0000}, //Four Four mode led order
{0b0001,0b0010,0b0100,0b0000,0b0000,0b0000,0b0000,0b0000}, //Three Four mode led order
{0b0001,0b0010,0b0100,0b1000,0b0100,0b0010,0b0000,0b0000}, //Six Eight mode led order
{0b0011,0b0100,0b1100,0b0010,0b0000,0b0000,0b0000,0b0000}, //Eighths mode led order
{0b1111,0b0010,0b0100,0b1000,0b1111,0b0100,0b0010,0b0001}}; //Sixteenths mode led order

const int maxTick[5] = {4,3,6,4,8};
const int updateRate[5] = {4,4,1,2,1};
const int beatRate[5] = {4,4,3,4,4};
class Metronome
{
public:
    Metronome(PinName led1, PinName led2, PinName led3, PinName led4, int time);
    int clockTick();
    void setTime(int type);
    void reset();
private:
    BusOut LEDS;
    int time;
    int tickNum;
    int dispNum;
};
 
Metronome::Metronome(PinName led1, PinName led2, PinName led3, PinName led4, int time)
    : LEDS(led1, led2, led3, led4), time(time), tickNum(0), dispNum(0){}

/*
* Updates the metronome.
* Changes the lights on the mbed depending on the time signature chosen.
* Returns 1 if the chord should change at this point.
* Returns 0 if the chord should not change.
*/ 
int Metronome::clockTick()
{
int returnVal = 0;
tickNum++;
if(tickNum%updateRate[time]==0) {
    dispNum++;
}
if(dispNum >= maxTick[time]) dispNum = 0;
if(tickNum%beatRate[time]==0) {
    returnVal = 1;
} 
LEDS = order[time][dispNum];
return returnVal;
}
/*
* Changes the time signature. 
* 0 = four/four time
* 1 = Three/four time
* 2 = Six/Eight time
* 3 = Eighth note mode
* 4 = Sixteenth note mode
*/
void Metronome::setTime(int type) {
    time = type;
}

void Metronome::reset() {
    tickNum = 0;
    dispNum = 0;
    time = 0;    
    LEDS = 0b0;
}

#pragma once
#include "config.h"
#include "melodies.h"
#include <vector>

class Buzzer{
public: 
    Buzzer();
    void playMelody(Melody *mld=0);
    //void playStop();
    void begin();
    void test();
    void loadMelodies();
    void setDefaultMelody(Melody *m){
        mldDefault = m;
    }
private:
    bool stoping = false;
    bool finished = true;
    Melody *mldCurrent;
    Melody *mldDefault = melodies[0];
    void _playMelody(Melody *mld);
    void loop();                // RTOS Task to perform all playing
};
extern Buzzer buzzer;
#ifndef VOICES_H
#define VOICES_H

#include <Arduino.h>
#include "oscillators.hpp"
#include "notes.hpp"
#include "envelopes.hpp"

class Basic_voice{
  public:
    Note note;
    Wavetable_oscillator osc0;
    ADSR env0;

    inline bool noteOn(byte ch, byte nt, byte vl) {
      if(env0.state == 4) {
        note.set_note(ch, nt, vl/127.0f);
        osc0.set_note(note);
        env0.state = 0;
        return true;
      }
      return false;
    }

    inline bool noteOff(byte ch, byte nt, byte vl) {
      if(note.check(ch, nt)) {
        env0.state = 3;
        return true;
      }
      return false;
    }

    inline void compute() {
      env0.compute();
      // Serial.print(osc0.step);
      // Serial.print(" ");
      // Serial.println(osc0.phase);
    }

    inline float process() {
      float sample = osc0.process();
      // Serial.printf("%f ",sample);
      sample = sample * note.velocity;
      //sample = sample * env0.envelope;
      // Serial.println(sample);
      return sample;
    }
};

#endif
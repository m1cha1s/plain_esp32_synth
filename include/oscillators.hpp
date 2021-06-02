#ifndef OSCILLATORS_H
#define OSCILLATORS_H

#include "config.hpp"
#include "globals.hpp"
#include "wavetables.hpp"
#include "notes.hpp"
#include "interpolators.hpp"

class Wavetable_oscillator {
  public:

    float step = 0;
    float phase = 0;
    const float *waveform = wavetables[0];

    // Wavetable_oscillator() {
    //   step = 0;
    //   phase = 0;
    // }

    Wavetable_oscillator(const float *wf = wavetables[0]) {
      waveform = wf;
    }

    inline void set_note(Note nt) {
      set_waveform(wavetables[nt.channel-1]);
      set_frequency(nt.pitch);
    }

    inline void set_waveform(const float *wf) {
      waveform = wf;
    }

    inline void set_frequency(float frequency) {
      step = (WAVETABLE_LEN/SAMPLE_RATE) * frequency;
      phase = 0;
    }

    inline float process() {
      int integral = phase;
      float fractional = phase-integral;
      float y0 = waveform[integral], y1 = waveform[integral+1], y2 = waveform[integral+2], y3 = waveform[integral+3];
      float sample = CubicInterpolate(y0, y1, y2, y3,fractional);
      phase += step;
      if(phase > WAVETABLE_LEN-1.0) phase -= WAVETABLE_LEN-1.0;
      // Serial.println(phase);
      return sample;
    }
};

#endif
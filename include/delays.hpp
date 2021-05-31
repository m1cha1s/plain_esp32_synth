#ifndef DELAY_H
#define DELAY_H

#include "config.hpp"
#include "mixers.hpp"

class Basic_delay {
    public :
        Mixer mx;

        float factor = 0.7;
        float delay_buffer[(int)(SAMPLE_RATE/2)];
        float delay_time = 1;
        int buffer_ptr = 0;
        int end_point = ((SAMPLE_RATE/2)-1)*delay_time;

        inline void set_factor(float wetness) {
            factor = wetness;
        }

        inline void set_delay_time(float time_s) {
            delay_time = time_s;
            end_point = ((SAMPLE_RATE/2)-1)*delay_time;
        }

        inline float process(float x) {
            mx.mix(x);
            mx.mix(delay_buffer[buffer_ptr]*factor);
            float sample = mx.process();
            delay_buffer[buffer_ptr] = sample;
            buffer_ptr++;
            if(buffer_ptr > end_point) buffer_ptr = 0;
            return sample;
        }
};

#endif
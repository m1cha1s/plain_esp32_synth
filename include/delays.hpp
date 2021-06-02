#ifndef DELAY_H
#define DELAY_H

#include "config.hpp"
#include "mixers.hpp"

class Basic_delay {
    public :
        Mixer mx;

        float wet = 0.7;
        std::vector<float> delay_buffer;
        float delay_time = 1;
        int buffer_ptr = 0;
        int end_point = ((SAMPLE_RATE/2)-1)*delay_time;

        Basic_delay() {
            delay_buffer.resize(SAMPLE_RATE/2);
        }

        inline void set_wet(float wetness) {
            wet = wetness;
        }

        inline void set_delay_time(float time_s) {
            delay_time = time_s;
            end_point = ((SAMPLE_RATE/2)-1)*delay_time;
        }

        inline float process(float x) {
            mx.mix(x);
            mx.mix(delay_buffer[buffer_ptr]*wet);
            float sample = mx.process();
            delay_buffer[buffer_ptr] = sample;
            buffer_ptr++;
            if(buffer_ptr > end_point) buffer_ptr = 0;
            return sample;
        }
};

#endif
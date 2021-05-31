#ifndef REVERB_H
#define REVERB_H

#include "config.hpp"
#include "filters.hpp"
#include "mixers.hpp"

class Reverb {
    public:

        float wet = 0.5;

        Mixer mx;

        Comb_filter cb0(int(SAMPLE_RATE_MS * 36.04), 0.805), 
                    cb1(SAMPLE_RATE_MS * 31.12, 0.827), 
                    cb2(SAMPLE_RATE_MS * 40.44, 0.783), 
                    cb3(SAMPLE_RATE_MS * 44.92, 0.764);
        All_pass_filter ap0(SAMPLE_RATE_MS * 5, 0.7), 
                        ap1(SAMPLE_RATE_MS * 1.68, 0.7), 
                        ap2(SAMPLE_RATE_MS * 0.48, 0.7);

        inline float process(float x) {
            mx.mix(cb0.process(x));
            mx.mix(cb1.process(x));
            mx.mix(cb2.process(x));
            mx.mix(cb3.process(x));
            float sample = ap2.process(ap1.process(ap0.process(mx.process())));
            return ((sample*wet) + (sample*(-wet)));
        }
};

#endif
#ifndef REVERB_H
#define REVERB_H

#include "config.hpp"
#include "filters.hpp"
#include "mixers.hpp"

class Reverb {
    private:

        const static int cb0_t = SAMPLE_RATE_MS * 36.04;
        const static int cb1_t = SAMPLE_RATE_MS * 31.12;
        const static int cb2_t = SAMPLE_RATE_MS * 40.44;
        const static int cb3_t = SAMPLE_RATE_MS * 44.92;

        const static int ap0_t = SAMPLE_RATE_MS * 5;
        const static int ap1_t = SAMPLE_RATE_MS * 1.68;
        const static int ap2_t = SAMPLE_RATE_MS * 0.48;

    public:

        float wet = 1;

        Mixer mx;

        Comb_filter cb0, 
                    cb1, 
                    cb2, 
                    cb3;
        All_pass_filter ap0, 
                        ap1, 
                        ap2;

        Reverb() {
            cb0.begin(cb0_t, 0.805);
            cb1.begin(cb1_t, 0.827);
            cb2.begin(cb2_t, 0.783);
            cb3.begin(cb3_t, 0.764);

            ap0.begin(ap0_t, 0.7);
            ap1.begin(ap1_t, 0.7);
            ap2.begin(ap2_t, 0.7);
        }

        inline float process(float x) {
            mx.mix(cb0.process(x));
            mx.mix(cb1.process(x));
            mx.mix(cb2.process(x));
            mx.mix(cb3.process(x));
            float sample = ap2.process(ap1.process(ap0.process(mx.process())));
            return ((sample*wet) + (x*(1-wet)));
        }
};

#endif
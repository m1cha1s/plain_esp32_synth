#ifndef MIXERS_H
#define MIXERS_H

class Mixer {
    public:
        int mixes = 0;
        float sample_mix = 0;

        inline void mix(float sample = 0) {
            sample_mix += sample;
            mixes++;
        }

        inline float process() {
            float sample = sample_mix/mixes;
            mixes = sample_mix = 0;
            return sample;
        }
};

#endif
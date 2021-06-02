#ifndef FILTERS_H
#define FILTERS_H

#include <math.h>
#include <Arduino.h>
#include "config.hpp"

// typedef struct {
//     float weight = 0.9;
//     float prev = 0;
// }Avrg_LP_Filter;

// inline float compute_Avrg_LP_Filter(float input, Avrg_LP_Filter *filter) {
//     float output = filter->weight*input + filter->weight*filter->prev;
//     filter->prev = output;
//     return output;
// }

// typedef struct {
//     float cutoff;
//     float resonance;
// }LP_Filter;


class Biquad_filter {
    public:
        float px, ppx;
        float py, ppy;
        float a0, a1, a2;
        float b1, b2;
        
        char type = 0;
        float Fc = 10000, Fs = SAMPLE_RATE, Q = 0.7, peak_gain = 1;
        float mod = 0;

        Biquad_filter(char tp = 0) {
            type = tp;
        }

        inline void set_cutoff(float cutoff) {
            Fc = cutoff;
            if (Fc < 30) Fc = 30;
        }

        inline void set_resonance(float resonance) {
            Q = resonance;
        }

        inline void set_peak_gain(float pg) {
            peak_gain = pg;
        }

        inline void compute() {
            float norm = 0;
            float V = pow(10, abs(peak_gain) / 20);
            float modulated = (Fc+mod);
            if (modulated < 30) Fc = 30;
            float K = tan(PI * modulated / Fs);
            switch(type) {
                case 0 :
                    norm = 1 / (1 + K / Q + K * K);
                    a0 = K * K * norm;
                    a1 = 2 * a0;
                    a2 = a0;
                    b1 = 2 * (K * K - 1) * norm;
                    b2 = (1 - K / Q + K * K) * norm;
                    break;
            }
        }

        inline float process(float x) {
            float y = 0;
            float phase1 = (x*a0) + (px*a1) + (ppx*a2);
            y = phase1 + (py*(-b1)) + (ppy*(-b2));
            ppx = px;
            px = x;
            ppy = py;
            py = y;
            return y;
        }

        inline void modulate(float m) {
            mod = m;
        }

};

class State_variable_filter {    
    public:

        char type = 0;
        float Fc = 10000, Fs = SAMPLE_RATE, Q = 0.1;

        float f, q;
        float low = 0, high = 0, band = 0, notch = 0;

        inline void set_type(char tp = 0) {
            type = tp;
        }

        inline void set_cutoff(float cutoff) {
            Fc = cutoff;
        }

        inline void set_resonance(float resonance) {
            Q = resonance;
        }

        inline void compute() {
            f = 2.0f*sin((PI*Fc)/Fs);
            q = 1.0f/Q;
        }

        inline float process(float x) {
            low = low + f * band;
            high = q * x - low - q*band;
            band = f * high + band;
            notch = high + low;
            switch(type) {
                case 0: return low;
                case 1: return high;
                case 2: return band;
                case 3: return notch;
                default: return low;
            }
        }
};

class Comb_filter {
    public:
        std::vector<float> delay_buff;
        float gain;
        int delay_ptr = 0;
        int delay_lim;

        void begin(int size, float gn) {
            delay_buff.resize(size);
            gain = gn;
            delay_lim = size-1;
        }

        inline float process(float x) {
            float sample = delay_buff[delay_ptr];
            delay_buff[delay_ptr] = x + (sample * gain);
            delay_ptr++;
            if(delay_ptr < delay_lim) delay_ptr = 0;
            return sample;
        }
};

class All_pass_filter {
    public:
        std::vector<float> delay_buff;
        float gain;
        int delay_ptr = 0;
        int delay_lim;

        void begin(int size, float gn) {
            delay_buff.resize(size);
            gain = gn;
            delay_lim = size-1;
        }

        inline float process(float x) {
            float sample = delay_buff[delay_ptr] + (-gain * x);
            delay_buff[delay_ptr] = x + (sample * gain);
            delay_ptr++;
            if(delay_ptr < delay_lim) delay_ptr = 0;
            return sample;
        }
};

#endif
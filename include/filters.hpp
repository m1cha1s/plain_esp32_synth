#ifndef FILTERS_H
#define FILTERS_H

#include <math.h>
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
    private:
        float px, ppx;
        float py, ppy;
        float a0, a1, a2;
        float b1, b2;
        
        char type = 0;
        float Fc = 1, Fs = SAMPLE_RATE, Q = 0.7, peak_gain = 1;


    public:

        Biquad_filter(char tp = 0) {
            type = tp;
        }

        inline void set_cutoff(float cutoff) {
            Fc = cutoff;
        }

        inline void set_resonance(float resonance) {
            Q = resonance;
        }

        inline void set_peak_gain(float pg) {
            peak_gain = pg;
        }

        inline void calculate() {
            float norm = 0;
            float V = pow(10, abs(peak_gain) / 20);
            float K = tan(PI * Fc / Fs);
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



};

class State_variable_filter {
    private:
        char type = 0;
        float Fc = 1, Fs = SAMPLE_RATE, Q = 0.7, peak_gain = 1;

        float f, q;
        float px, py;
    
    public:
        inline void calculate() {
            f = 2.0f*sin((PI*Fc)/Fs);
            q = 1.0f/Q;
        }

        inline float process(float x) {
    
        }
};

typedef struct {
    
}State_variable_filter;

#endif
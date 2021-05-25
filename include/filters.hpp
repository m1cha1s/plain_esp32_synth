#ifndef FILTERS_H
#define FILTERS_H

#include <math.h>
#include "config.hpp"

typedef struct {
    float weight = 0.9;
    float prev = 0;
}Avrg_LP_Filter;

inline float compute_Avrg_LP_Filter(float input, Avrg_LP_Filter *filter) {
    float output = filter->weight*input + filter->weight*filter->prev;
    filter->prev = output;
    return output;
}

typedef struct {
    float cutoff;
    float resonance;
}LP_Filter;


typedef struct {

    char type = 0;
    float Fc = 1, Fs = SAMPLE_RATE, Q = 0.7, peak_gain = 1;

    float px, ppx;
    float py, ppy;
    float a0, a1, a2;
    float b1, b2;
}Biquad;

inline void calculate_biquad(Biquad *bq) {
    float norm = 0;
    float V = pow(10, abs(bq->peak_gain) / 20);
    float K = tan(PI * bq->Fc / bq->Fs);
    switch(bq->type) {
        case 0 :
            norm = 1 / (1 + K / bq->Q + K * K);
            bq->a0 = K * K * norm;
            bq->a1 = 2 * bq->a0;
            bq->a2 = bq->a0;
            bq->b1 = 2 * (K * K - 1) * norm;
            bq->b2 = (1 - K / bq->Q + K * K) * norm;
            break;
    }
}

inline float do_biquad(float x, Biquad *bq) {
    float y = 0;
    float phase1 = (x*bq->a0) + (bq->px*bq->a1) + (bq->ppx*bq->a2);
    y = phase1 + (bq->py*(-bq->b1)) + (bq->ppy*(-bq->b2));
    bq->ppx = bq->px;
    bq->px = x;
    bq->ppy = bq->py;
    bq->py = y;
    return y;
}

typedef struct {
    char type = 0;
    float Fc = 1, Fs = SAMPLE_RATE, Q = 0.7, peak_gain = 1;

    float f, q;
    float px, py;
}State_variable_filter;

inline void calculate_state_variable_filter(State_variable_filter * svf) {
    svf->f = 2.0f*sin((PI*svf->Fc)/svf->Fs);
    svf->q = 1.0f/svf->Q;
}

inline float do_state_variable_filter(float x, State_variable_filter * svf) {
    
}

#endif
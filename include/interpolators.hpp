#ifndef INTERPOLATORS_H
#define INTERPOLATORS_H

inline float CubicInterpolate(
   float y0,float y1,
   float y2,float y3,
   float mu)
{
   float a0,a1,a2,a3,mu2;

   mu2 = mu*mu;
   a0 = y3 - y2 - y0 + y1;
   a1 = y0 - y1 - a0;
   a2 = y2 - y0;
   a3 = y1;

   return(a0*mu*mu2+a1*mu2+a2*mu+a3);
}

inline float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}


#endif
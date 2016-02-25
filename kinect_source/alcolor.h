#ifndef ALCOLOR_H
#define ALCOLOR_H

#include <math.h>

class ALColor
{
public:
    ALColor();

    void setHsb(float hue, float saturation, float brightness, float alpha);
    void set(float _gray, float _a);
    float r;
    float g;
    float b;
    float a;
};

#endif // ALCOLOR_H

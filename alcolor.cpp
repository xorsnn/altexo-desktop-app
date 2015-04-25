#include "alcolor.h"

float ofClamp(float value, float min, float max) {
        return value < min ? min : value > max ? max : value;
}

int LIMIT = 255;

ALColor::ALColor()
{
    this->r = 0;
    this->g = 0;
    this->b = 0;
    this->a = 255;

}

void ALColor::set(float _gray, float _a){
    this->r = _gray;
    this->g = _gray;
    this->b = _gray;
    this->a = _a;

    int l =0;
    l+=1;

}

void ALColor::setHsb(float hue, float saturation, float brightness, float alpha) {
    saturation = ofClamp(saturation, 0, LIMIT);
    brightness = ofClamp(brightness, 0, LIMIT);
    if(brightness == 0) { // black
        this->set(0,0);

    } else if(saturation == 0) { // grays
        this->set(brightness, 255);
    } else {
        float hueSix = hue * 6.f / LIMIT;
        float saturationNorm = saturation / LIMIT;
        int hueSixCategory = (int) floorf(hueSix);
        float hueSixRemainder = hueSix - hueSixCategory;
        int pv = (int) ((1.f - saturationNorm) * brightness);
        int qv = (int) ((1.f - saturationNorm * hueSixRemainder) * brightness);
        int tv = (int) ((1.f - saturationNorm * (1.f - hueSixRemainder)) * brightness);
        switch(hueSixCategory) {
        case 0: case 6: // r
            r = brightness;
            g = tv;
            b = pv;
            break;
        case 1: // g
            r = qv;
            g = brightness;
            b = pv;
            break;
        case 2:
            r = pv;
            g = brightness;
            b = tv;
            break;
        case 3: // b
            r = pv;
            g = qv;
            b = brightness;
            break;
        case 4:
            r = tv;
            g = pv;
            b = brightness;
            break;
        case 5: // back to r
            r = brightness;
            g = pv;
            b = qv;
            break;
        }
    }

    // finally assign the alpha
    a = alpha;
}

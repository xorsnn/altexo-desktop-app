#ifndef HSV2RGB_H
#define HSV2RGB_H

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB_t;

typedef struct {
    uint8_t h;
    uint8_t s;
    uint8_t v;
} HSV_t;

const uint8_t max_whiteness = 15;
const uint8_t max_value = 17;

const uint8_t sixth_hue = 16;
const uint8_t third_hue = sixth_hue * 2;
const uint8_t half_hue = sixth_hue * 3;
const uint8_t two_thirds_hue = sixth_hue * 4;
const uint8_t five_sixths_hue = sixth_hue * 5;
const uint8_t full_hue = sixth_hue * 6;

inline RGB_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    return (RGB_t) {r, g, b};
}

inline HSV_t hsv(uint8_t h, uint8_t s, uint8_t v) {
    return (HSV_t) {h, s, v};
}

const RGB_t black = {0, 0, 0};

//RGB_t hsv2rgb(HSV_t hsv) {
//    if (hsv.v == 0) return black;

//    uint8_t high = hsv.v * max_whiteness;//channel with max value
//    if (hsv.s == 0) return rgb(high, high, high);

//    uint8_t W = max_whiteness - hsv.s;
//    uint8_t low = hsv.v * W;//channel with min value
//    uint8_t rising = low;
//    uint8_t falling = high;

//    uint8_t h_after_sixth = hsv.h % sixth_hue;
//    if (h_after_sixth > 0) {//not at primary color? ok, h_after_sixth = 1..sixth_hue - 1
//        uint8_t z = hsv.s * uint8_t(hsv.v * h_after_sixth) / sixth_hue;
//        rising += z;
//        falling -= z + 1;//it's never 255, so ok
//    }

//    uint8_t H = hsv.h;
//    while (H >= full_hue) H -= full_hue;

//    if (H < sixth_hue) return rgb(high, rising, low);
//    if (H < third_hue) return rgb(falling, high, low);
//    if (H < half_hue) return rgb(low, high, rising);
//    if (H < two_thirds_hue) return rgb(low, falling, high);
//    if (H < five_sixths_hue) return rgb(rising, low, high);
//    return rgb(high, low, falling);
//    return black;
//}

#endif // HSV2RGB_H

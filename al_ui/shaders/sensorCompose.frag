#version 120
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2D textureMap;
uniform usampler2D depthTexMap;
varying vec2 vUV;

const float FLT_EPSILON = 1.19209290E-07F;
const float LIMIT = 255.0;
const float MAX_DEPTH = 1005.0;
const float MIN_DEPTH = 555.0;

float ofMap(float value, float inputMin, float inputMax, float outputMin,
            float outputMax, bool clamp) {
  if (abs(inputMin - inputMax) < FLT_EPSILON) {
    return outputMin;
  } else {
    float outVal =
        ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) +
         outputMin);
    if (clamp) {
      if (outputMax < outputMin) {
        if (outVal < outputMax)
          outVal = outputMax;
        else if (outVal > outputMin)
          outVal = outputMin;
      } else {
        if (outVal > outputMax)
          outVal = outputMax;
        else if (outVal < outputMin)
          outVal = outputMin;
      }
    }
    return outVal;
  }
}

float ofClamp(float value, float min, float max) {
  return value < min ? min : value > max ? max : value;
}

vec4 getHsb(float hue, float saturation, float brightness, float alpha) {
  float r = 0.0;
  float g = 0.0;
  float b = 0.0;
  saturation = ofClamp(saturation, 0, LIMIT);
  brightness = ofClamp(brightness, 0, LIMIT);
  if (brightness == 0) { // black
    return vec4(0.0, 0.0, 0.0, 0.0);
  } else if (saturation == 0) { // grays
    float brightnessNorm = brightness / LIMIT;
    return vec4(brightnessNorm, brightnessNorm, brightnessNorm, 1.0);
  } else {
    float hueSix = hue * 6.f / LIMIT;
    float saturationNorm = saturation / LIMIT;
    int hueSixCategory = int(floor(hueSix));
    float hueSixRemainder = hueSix - hueSixCategory;
    int pv = int((1.f - saturationNorm) * brightness);
    int qv = int((1.f - saturationNorm * hueSixRemainder) * brightness);
    int tv = int((1.f - saturationNorm * (1.f - hueSixRemainder)) * brightness);
    switch (hueSixCategory) {
    case 0:
    case 6: // r
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
  return vec4(r / LIMIT, g / LIMIT, b / LIMIT, 1.0);
}

vec4 huePixelForDepth(float distanceMm) {
  if (distanceMm == 0.0 || distanceMm < MIN_DEPTH || distanceMm > MAX_DEPTH) {
    return vec4(0.0, 0.0, 0.0, 1.0);
  }
  float alignedPix = ofMap(distanceMm, MIN_DEPTH, MAX_DEPTH, 0, 255, true);
  return getHsb(alignedPix, 255.0, 255.0, 255.0);
}

void main() {
  if (vUV.x > 1.0) {
    vec2 vUV2 = vUV;
    vUV2.x = vUV2.x - 1.0;
    vUV2.y /= 2;
    gl_FragColor = texture2D(textureMap, vUV2);
  } else {
    vec2 vUV1 = vUV;
    vUV1.y /= 2;
    uint val = texture2D(depthTexMap, vUV1).x;
    gl_FragColor = huePixelForDepth(float(val));
  }
}

#version 120

attribute vec2 vVertex; // object space vertex position
attribute vec2 vTexCoord;

// uniform
uniform mat4 MVP; // combined modelview projection matrix

varying vec2 vUV;
uniform sampler2D textureMap;
varying float cond;

vec3 rgb2hsl(vec3 color) {
  float h = 0.0;
  float s = 0.0;
  float l = 0.0;
  float r = color.r;
  float g = color.g;
  float b = color.b;
  float cMin = min(r, min(g, b));
  float cMax = max(r, max(g, b));
  l = (cMax + cMin) / 2.0;
  if (cMax > cMin) {
    float cDelta = cMax - cMin;
    // saturation
    if (l < 0.5) {
      s = cDelta / (cMax + cMin);
    } else {
      s = cDelta / (2.0 - (cMax + cMin));
    }
    // hue
    if (r == cMax) {
      h = (g - b) / cDelta;
    } else if (g == cMax) {
      h = 2.0 + (b - r) / cDelta;
    } else {
      h = 4.0 + (r - g) / cDelta;
    }
    if (h < 0.0) {
      h += 6.0;
    }
    h = h / 6.0;
  }
  return vec3(h, s, l);
}

const float minD = 555.0;
const float maxD = 1005.0;

const float fx = 1.094017094017094;  // 640.0/(( maxD + minD ) / 2)*640/480;
const float fy = 0.8205128205128206; // 480.0/(( maxD + minD ) / 2)*640/480;

// taken from freenect example
const float wAmount = 320.0; // TODO move to uniform
const float hAmount = 240.0; // TODO move to uniform
// const float f = 595.0/640.0; // devide by wAmoutn to normalize
const float f = 595.0; // devide by wAmoutn to normalize

varying vec2 vUv;
varying float visibility;

vec3 xyz(float x, float y, float depth) {
  // float m_depth = depth/640.0;
  // float m_depth = depth;
  // ====== current =======
  // float z = depth * (maxD - minD) + minD;
  // return vec3((x / 640.0) * z * fx, (y / 480.0) * z * fy, -z); // my
  // // return vec3( ( x / 640.0 ) * z, ( y / 640.0 ) * z, -z); //my
  // // return vec3(x  * z * fx, y * z * fy, -z); // my
  // ===== ~current =======
  //
  // float z = depth * (maxD - minD) + minD;
  float outputMin = 0.0;
  float outputMax = 1.0;
  float inputMax = maxD;
  float inputMin = minD;

  // float z =
  //     (depth - outputMin) / (outputMax - outputMin) * (inputMax - inputMin) +
  //     inputMin;

  float z =
      ((depth - outputMin) / (outputMax - outputMin)) * (inputMax - inputMin) + inputMin;

  return vec3(x * (wAmount) * z / f, // X = (x - cx) * d / fx
              y * (wAmount) * z / f, // Y = (y - cy) * d / fy
              -z + inputMin);                       // Z = d
  // return vec3(x * z / f, // X = (x - cx) * d / fx
  //             y * z / f, // Y = (y - cy) * d / fy
  //             z);                        // Z = d

  // ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) +
  //  outputMin);
  // v = (value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) +
  // outputMin;
  // v - outputMin = (value - inputMin) / (inputMax - inputMin) * (outputMax -
  // outputMin)
  // (v - outputMin)/(outputMax - outputMin) *(inputMax - inputMin) + inputMin =
  // value
}

void main() {
  // vec3 pos = position;
  visibility = 1.0;

  vUv = vec2(vTexCoord.x, 1 - vTexCoord.y);
  vUv.x = vUv.x * 0.5;
  vec3 hsl = rgb2hsl(texture2D(textureMap, vUv).xyz);
  vUv.x += 0.5;
  visibility = hsl.z * 2.0;

  vec3 pos = xyz(vVertex.x, vVertex.y, hsl.x);
  // pos.z += (maxD - minD) / 3.0 + minD;

  // get the clip space position by multiplying the combined MVP matrix with the
  // object space
  // vertex position

  // 1)
  // gl_Position = MVP * vec4(vVertex.x, vVertex.y, pos.z / 640, 1.0); // OLD
  gl_Position = MVP * vec4(pos.x, pos.y, pos.z, 1.0);
  // gl_Position = MVP * vec4(vVertex.x*10.0, vVertex.y*10.0,-hsl.x*10, 1.0);
  // 2)
  // gl_Position = vec4(vVertex.x, vVertex.y, 0, 1.0);
  vUV = vec2(vTexCoord.x * 0.5 + 0.5, 1 - vTexCoord.y);
  gl_PointSize = 2.0;
}

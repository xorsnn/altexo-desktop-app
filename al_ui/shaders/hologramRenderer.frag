#version 120

uniform sampler2D textureMap;
varying vec2 vUV;
varying float visibility;
varying float cond;

void main() {
  if (visibility > 0.95) {
    gl_FragColor = texture2D(textureMap, vUV);
  } else {
    discard;
  }
}

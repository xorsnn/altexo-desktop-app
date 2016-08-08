#version 120

uniform sampler2D textureMap;
varying vec2 vUV;

void main() {
  // gl_FragColor = texture2D(textureMap, vUV);
  gl_FragColor = vec4(0.8, 0.8, 0.8, 1.0);
}

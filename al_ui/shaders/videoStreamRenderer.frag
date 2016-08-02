#version 120

uniform sampler2D textureMap;
varying vec2 vUV;

void main() {
  // gl_FragColor = texture2D(textureMap, vUV);
  gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);
}

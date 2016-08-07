#version 120

uniform sampler2D textureMap;
varying vec2 vUV;
varying float visibility;
varying float cond;

void main() {

  // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  if (visibility > 0.95) {
  //   // if (cond > 1.0) {
  //   //   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  //   // } else {
    gl_FragColor = texture2D(textureMap, vUV);
  //   // }
  } else {
    discard;
  }
}

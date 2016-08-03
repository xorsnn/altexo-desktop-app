#version 120

attribute vec3 vVertex; // object space vertex position
uniform mat4 MVP; // combined modelview projection matrix

varying vec2 vUV;
void main() {
  // get the clip space position by multiplying the combined MVP matrix with the
  // object space
  gl_Position = MVP * vec4(vVertex.xy, 0.0, 1.0);
  vUV = vec2(vVertex.x + 0.5, 1 - (vVertex.y + 0.5));
}

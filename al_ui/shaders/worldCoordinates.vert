#version 120

attribute vec3 vVertex; // object space vertex position
attribute vec3 vColor; // object space vertex position
// uniform
uniform mat4 MVP; // combined modelview projection matrix

varying vec3 col;

void main() {
  gl_Position = MVP * vec4(vVertex, 1.0);
  col = vColor;
}

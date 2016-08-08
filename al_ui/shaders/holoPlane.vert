#version 120

attribute vec3 vVertex; // object space vertex position
attribute vec2 vTexCoord;
uniform mat4 MVP; // combined modelview projection matrix

void main() {
  // get the clip space position by multiplying the combined MVP matrix with the
  // object space
  gl_Position = MVP * vec4(vVertex, 1.0);
}

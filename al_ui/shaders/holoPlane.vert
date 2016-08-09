#version 330 core

layout(location=0) in vec3 vVertex; // object space vertex position
// layout(location=1) in vec2 vTexCoord;
uniform mat4 MVP; // combined modelview projection matrix

void main() {
  // get the clip space position by multiplying the combined MVP matrix with the
  // object space
  gl_Position = MVP * vec4(vVertex, 1.0);
}

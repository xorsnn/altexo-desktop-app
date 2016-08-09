#version 330 core

// attribute vec2 vVertex; // object space vertex position
// attribute vec2 vTexCoord;

layout(location=0) in vec2 vVertex; // object space vertex position
layout(location=1) in vec2 vTexCoord;

uniform mat4 MVP; // combined modelview projection matrix
smooth out vec2 vUV;

void main() {
  // get the clip space position by multiplying the combined MVP matrix with the
  // object space
  gl_Position = MVP * vec4(vVertex, 0.0, 1.0);
  vUV = vec2(vTexCoord.x, 1 - vTexCoord.y);
}

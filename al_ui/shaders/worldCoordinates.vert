#version 330 core

layout(location=0) in vec3 vVertex; // object space vertex position
layout(location=1) in vec3 vColor; // object space vertex position

// uniform
uniform mat4 MVP; // combined modelview projection matrix

smooth out vec4 vSmoothColor;

void main() {
  gl_Position = MVP * vec4(vVertex, 1.0);
  vSmoothColor = vec4(vColor, 1);
}

#version 330 core

smooth in vec4 vSmoothColor;
layout(location=0) out vec4 vFragColor; // object space vertex position

void main() {
  vFragColor = vSmoothColor;
}

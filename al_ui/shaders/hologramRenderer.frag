#version 330 core

layout(location=0) out vec4 vFragColor;

uniform sampler2D textureMap;

in vec2 vUV;
in float visibility;

void main() {
  if (visibility > 0.95) {
    vFragColor = texture(textureMap, vUV);
  } else {
    discard;
  }
}

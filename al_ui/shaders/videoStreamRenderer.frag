#version 330 core

layout(location=0) out vec4 vFragColor;
uniform sampler2D textureMap;
smooth in vec2 vUV;

void main() {
  // gl_FragColor = texture(textureMap, vUV);
  vFragColor = texture(textureMap, vUV);
}

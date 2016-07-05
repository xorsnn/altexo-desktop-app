#version 120
// #extension GL_EXT_gpu_shader4 : enable
// layout(location=0) out vec4 vFragColor;	//fragment shader output

//input form the vertex shader
varying vec4 vSmoothColor;		//interpolated colour to fragment shader

uniform sampler2D textureMap;
uniform sampler2D depthTexMap;
varying vec2 vUV;

void main()
{
	if (vUV.x > 0.5) {
		vec2 vUV1 = vUV;
		vUV1.x = (vUV1.x-0.5)*2;
		float val = texture2D(depthTexMap, vUV1).r;
		if (val > 0.0) {
			gl_FragColor = vec4(val, 1.0, 1.0, 1.0);
		} else {
			gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
		}
	} else {
		vec2 vUV2 = vUV;
		vUV2.x*=2;
		gl_FragColor = texture2D(textureMap, vUV2);
	}
}

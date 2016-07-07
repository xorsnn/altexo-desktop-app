#version 120
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2D textureMap;
uniform usampler2D depthTexMap;
varying vec2 vUV;

void main()
{
	if (vUV.x > 1.0) {
		vec2 vUV1 = vUV;
		vUV1.x = (vUV1.x-1.0);
		vUV1.y /= 2;
		uint val = texture2D(depthTexMap, vUV1).x;
		uint cmp = uint(1500);
		if (val > cmp) {
			gl_FragColor = vec4(0.5, 1.0, 1.0, 1.0);
		} else {
			gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
		}
	} else {
		vec2 vUV2 = vUV;
		vUV2.y /= 2;
		gl_FragColor = texture2D(textureMap, vUV2);
	}
}

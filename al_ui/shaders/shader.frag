#version 120
// #extension GL_EXT_gpu_shader4 : enable

uniform sampler2D textureMap;
// uniform usampler2D depthTexMap;
varying vec2 vUV;

void main()
{
	if ((vUV.x < 0.05) ||
			(vUV.x > 0.95) ||
			(vUV.y < 0.1) ||
			(vUV.y > 0.9)) {
		gl_FragColor = vec4(0.1, 0.1, 0.1, 1.0);
	} else {
		// gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
		gl_FragColor = texture2D(textureMap, vUV);
	}
}

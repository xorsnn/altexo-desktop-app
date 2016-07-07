#version 120
// #extension GL_EXT_gpu_shader4 : enable

uniform sampler2D textureMap;
// uniform usampler2D depthTexMap;
varying vec2 vUV;

void main()
{
	// if ((vUV.x < 0.01) ||
	// 		(vUV.x > 0.99) ||
	// 		(vUV.y < 0.05) ||
	// 		(vUV.y > 0.95)) {
	// 	gl_FragColor = vec4(0.1, 0.1, 0.1, 1.0);
	// } else {
		gl_FragColor = texture2D(textureMap, vUV);
	// }
}

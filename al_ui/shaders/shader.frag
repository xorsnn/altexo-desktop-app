#version 120

// layout(location=0) out vec4 vFragColor;	//fragment shader output

//input form the vertex shader
varying vec4 vSmoothColor;		//interpolated colour to fragment shader

uniform sampler2D textureMap;
varying vec2 vUV;

void main()
{
	//set the interpolated colour as the shader output
	// gl_FragColor = vSmoothColor;
	gl_FragColor = texture2D(textureMap, vUV);
}

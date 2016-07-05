#version 120

attribute vec3 vVertex;	//object space vertex position
attribute vec3 vColor;	//per-vertex colour

//output from the vertex shader
varying vec4 vSmoothColor;		//smooth colour to fragment shader

//uniform
uniform mat4 MVP;	//combined modelview projection matrix

void main()
{
	//assign the per-vertex colour to vSmoothColor varying
   vSmoothColor = vec4(vColor,1);

   //get the clip space position by multiplying the combined MVP matrix with the object space
   //vertex position
   gl_Position = MVP*vec4(vVertex,1);
}

#version 120

attribute vec3 vVertex;	//object space vertex position
varying vec2 vUV;
//uniform
uniform mat4 MVP;	//combined modelview projection matrix

void main()
{
   //get the clip space position by multiplying the combined MVP matrix with the object space
   //vertex position
   gl_Position = MVP*vec4(vVertex,1);
   vUV = vec2(vVertex.x + 1.0, vVertex.y + 1.0);
}

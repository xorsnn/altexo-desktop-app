#version 330 core
layout(location=0) in vec3 vVertex;	//object space vertex position
//uniform
uniform mat4 MVP;	//combined modelview projection matrix

smooth out vec2 vUV;

void main()
{
   //get the clip space position by multiplying the combined MVP matrix with the object space
   //vertex position
   gl_Position = MVP*vec4(vVertex,1);
   vUV = vec2(vVertex.x + 1.0, vVertex.y + 1.0);
}

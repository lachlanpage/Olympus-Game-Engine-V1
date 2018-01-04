#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 textureCoordinates;

uniform mat4 biasMatrix;

uniform mat4 model;
uniform mat4 view; 
uniform mat4 projection;
uniform mat4 depthMVP;

out vec2 UV;
out vec4 shadowCoord;

void main()
{
	mat4 depthBiasMVP = biasMatrix*( projection * view * model);
	shadowCoord  = depthBiasMVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    gl_Position = vec4(aPos.x,aPos.y, aPos.z, 1.0);
	UV = textureCoordinates;
}
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 textureCoordinates;
layout (location = 2) in vec3 normalData;

uniform mat4 model;
uniform mat4 view; 
uniform mat4 projection;

out vec2 vs_textureCoordinates;
out vec3 vs_normalData;
out vec3 vs_pos;
void main()
{
    gl_Position = projection * view * model *vec4(aPos.x,aPos.y, aPos.z, 1.0);
	vs_pos = aPos;
	vs_normalData = normalData;
	vs_textureCoordinates = textureCoordinates;

}
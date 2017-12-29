#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 textureCoordinates;


uniform vec3 Position;
uniform float Radius;

uniform mat4 view; 
uniform mat4 projection;
uniform mat4 model;

out vec2 UV;
out vec4 fsPos;
out vec4 vs_pos;


void main()
{
	vec4 pos = projection*view*model*vec4(aPos.x,aPos.y,aPos.z,1.0);
    gl_Position = pos;
	vs_pos = pos;
	UV = textureCoordinates;
}
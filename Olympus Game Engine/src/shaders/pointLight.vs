#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view; 
uniform mat4 projection;

out vec4 vs_pos;
out mat4 inverseProjView;
float lightRad = 5;

void main()
{
	vec4 pos = projection * view * model * vec4(aPos,1.0);
    gl_Position = pos;
	vs_pos = pos;
	inverseProjView = inverse(projection*view);
}
#version 330 

in vec2 position;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main(void){
	gl_Position = projection * view * model * vec4(position, 0.0, 1.0);
}
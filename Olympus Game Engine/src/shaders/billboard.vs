#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 textureCoordinates;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 viewProjection;
uniform mat4 viewModel;

out vec2 UV;

void main(void){
	gl_Position = projection * viewModel * vec4(aPos, 1.0);
	UV = textureCoordinates;
}
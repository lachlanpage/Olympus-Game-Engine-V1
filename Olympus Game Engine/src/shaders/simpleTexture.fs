#version 430 core 

in vec2 UV;

layout (location = 0) out vec3 color;

uniform sampler2D renderedTexture;
uniform sampler2D normalTexture;


void main(){
	color =texture(renderedTexture, UV).xyz;
}


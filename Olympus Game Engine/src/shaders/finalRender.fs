#version 430 core 
//this is for quad that renders final stuff 
in vec2 UV;
in vec4 shadowCoord;

layout (location = 0) out vec4 color;

uniform sampler2D renderedImage;


void main(){
	color = texture(renderedImage, UV);
}


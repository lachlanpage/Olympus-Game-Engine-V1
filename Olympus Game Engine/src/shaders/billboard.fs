#version 330 core
out vec4 color;

in vec2 UV;
uniform sampler2D grassTexture;

void main(void){
	color = texture(grassTexture, UV);
	//color = vec4(1.0,1.0,1.0,1.0);
}
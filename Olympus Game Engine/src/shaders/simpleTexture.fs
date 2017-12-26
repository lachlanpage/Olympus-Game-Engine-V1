#version 430 core 

in vec2 UV;

layout (location = 0) out vec4 color;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D positionTexture;

uniform float textureSelector;

void main(){
	if(textureSelector == 0){
		color =texture(colorTexture, UV);
	} 
	
	if(textureSelector == 1){
		color =texture(normalTexture, UV);
	}

	if(textureSelector == 2){
		color =texture(positionTexture, UV);
	}
}


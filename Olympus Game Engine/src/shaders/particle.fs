#version 330 

out vec4 color;

in vec2 textureCoords1;
in vec2 textureCoords2;
in float blend;

in vec2 textureCoord;

uniform sampler2D particleTexture;

void main(void){
	//color = vec4(1.0,1.0,1.0,1.0);
	//color = texture(particleTexture, textureCoord);

	vec4 col1 = texture(particleTexture, textureCoords1);
	vec4 col2 = texture(particleTexture, textureCoords2);

	color = mix(col1, col2, blend);
}
#version 330 

in vec2 position;
in vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec2 textureCoords1;
out vec2 textureCoords2;
out float blend;

uniform vec2 texOffset1;
uniform vec2 texOffset2;
uniform vec2 texCoordInfo;

out vec2 textureCoord;

void main(void){
	gl_Position = projection * view * model * vec4(position, 0.0, 1.0);
	textureCoord = position + vec2(0.5,0.5);
	textureCoord.y = 1.0 - textureCoord.y;

	//textureCoord /= texCoordInfo.x;
	//textureCoords1 = textureCoord + texOffset1;
	//textureCoords2 = textureCoord + texOffset2;
	//blend = texCoordInfo.y;
}
#version 330 

layout (location = 0) in  vec2 position;

layout (location = 1) in vec4 col1;
layout (location = 2) in vec4 col2;
layout (location = 3) in vec4 col3;
layout (location = 4) in vec4 col4;

layout (location = 5) in vec4 texOffset;
layout (location = 6) in float blendFac;
//in vec4 texOffset;
//in float blendFac;

uniform mat4 projection;
//uniform mat4 model;
uniform mat4 viewProjection;

uniform mat4 view;

out vec2 textureCoords1;
out vec2 textureCoords2;
out float blend;

//uniform vec4 texOffset;
//uniform float blendFac;


uniform float numRows;

void main(void){
	mat4 model = mat4(col1,col2,col3,col4);
	gl_Position = viewProjection* model * vec4(position, 0.0, 1.0);
	//gl_Position = vec4(position, 0.0, 1.0);
	vec2 textureCoord = position + vec2(0.5,0.5);
	textureCoord.y = 1.0 - textureCoord.y;
	textureCoord /= numRows;
	textureCoords1 = textureCoord + texOffset.xy;
	textureCoords2 = textureCoord + texOffset.zw;
	blend = blendFac;


	//textureCoord /= texCoordInfo.x;
	//textureCoords1 = textureCoord + texOffset.xy;
	//textureCoords2 = textureCoord + texOffset.zw;
	//blend = texCoordInfo.y;
}
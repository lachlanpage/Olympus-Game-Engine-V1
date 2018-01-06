#version 430 core

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 normalData;
layout (location = 2) out vec4 positionData;

uniform sampler2D albedo_texture;
uniform bool isSelected;

in vec2 vs_textureCoordinates;
in vec3 vs_normalData;
in vec3 vs_pos;

void main()
{
    color = texture(albedo_texture, vs_textureCoordinates);//vec4(0.5f, 0.5f, 0.5f, 1.0f);
	//textureCoordinates = vec4(vs_textureCoordinates, 0.0,1.0);
	if(isSelected){
		color = vec4(1.0,1.0,1.0,1.0);
	}
	normalData = vec4(vs_normalData, 1.0);
	positionData = vec4(vs_pos, 1.0);
} 
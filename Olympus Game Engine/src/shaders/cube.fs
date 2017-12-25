#version 430 core

layout (location = 0) out vec4 color;
layout (location = 1) out vec2 textureCoordinates;
layout (location = 2) out vec3 normalData;

in vec2 vs_textureCoordinates;
in vec3 vs_normalData;

void main()
{
    color = vec4(sin(vs_textureCoordinates.x), 1.0f, 1.0f, 1.0f);
	textureCoordinates = vs_textureCoordinates;
	//noramlData = vs_normalData;
} 
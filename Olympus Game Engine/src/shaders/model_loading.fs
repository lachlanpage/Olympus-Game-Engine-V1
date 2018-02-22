#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 normalData;
layout (location = 2) out vec4 positionData;
layout (location = 3) out vec4 specularData;


in vec2 TexCoords;
in vec2 vs_textureCoordinates;
in vec3 vs_normalData;
in vec3 vs_pos;

uniform sampler2D texture_diffuse1;
uniform sampler2D specular_texture;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
	//FragColor = vec4(1.0,1.0,0.0,1.0);
	specularData = texture(specular_texture, vs_textureCoordinates);
	normalData = vec4(vs_normalData, 1.0);
	positionData = vec4(vs_pos, 1.0);
}

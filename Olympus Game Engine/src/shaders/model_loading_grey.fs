#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 normalData;
layout (location = 2) out vec4 positionData;
layout (location = 3) out vec4 specularData;
layout (location = 4) out vec4 eyeNormalData;
layout (location = 5) out vec4 eyePositionData;

in vec2 TexCoords;
in vec2 vs_textureCoordinates;
in vec3 vs_normalData;
in vec3 vs_pos;

in vec3 eyePos;
in vec3 eyeNormal;

uniform sampler2D texture_diffuse1;
uniform sampler2D specular_texture;

uniform int isSelected;

void main()
{    
    FragColor = vec4(0.5,0.5,0.5,1);
	specularData = texture(specular_texture, vs_textureCoordinates);
	normalData = vec4(vs_normalData, 1.0);
	positionData = vec4(vs_pos, 1.0);

	eyePositionData = vec4(eyePos,1.0);
	eyeNormalData = vec4(eyeNormal,1.0);
}

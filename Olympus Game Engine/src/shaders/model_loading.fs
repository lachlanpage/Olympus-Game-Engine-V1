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
in mat3 TBN;

uniform sampler2D texture_diffuse1;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;

uniform int isSelected;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);

	specularData = texture(specular_texture, vs_textureCoordinates);
	
	vec3 normal = texture(normal_texture, TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normalData = vec4(normalize(normal),1.0);

	positionData = vec4(vs_pos, 1.0);

	eyePositionData = vec4(eyePos,1.0);
	eyeNormalData = vec4(eyeNormal,1.0);
}

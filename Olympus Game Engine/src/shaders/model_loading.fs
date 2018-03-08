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

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;

uniform sampler2D texture_specular0;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;
uniform sampler2D texture_specular4;

uniform sampler2D texture_normal0;
uniform sampler2D texture_normal1;
uniform sampler2D texture_normal2;
uniform sampler2D texture_normal3;
uniform sampler2D texture_normal4;

uniform sampler2D texture_displacement0;

in vec3 tangentViewPos;
in vec3 tangentFragPos;

in vec3 aTan;
in vec3 aBitan;
in vec3 aNorm;

uniform sampler2D specular_texture;
uniform sampler2D normal_texture;

uniform int normalMapPresent;
uniform int displacementMapPresent;

uniform int isSelected;

float height_scale = 0.1;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
	int minLayers = 8;
	int maxLayers = 32;
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0,0.0,1.0), viewDir)));
	float layerDepth = 1.0 / numLayers;
	float currentLayerDepth = 0.0;

	//shift 
	vec2 p = viewDir.xy / viewDir.z * height_scale;
	vec2 deltaTexCoords = p / numLayers;

	//initial values 
	vec2 currentTexCoords = texCoords;
	float currentDepthMapValue = texture(texture_displacement0, currentTexCoords).r;

	while(currentLayerDepth < currentDepthMapValue){
		currentTexCoords -= deltaTexCoords;
		currentDepthMapValue = texture(texture_displacement0, currentTexCoords).r;
		currentLayerDepth += layerDepth;
	}


	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	float afterDepth = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(texture_displacement0, prevTexCoords).r - currentLayerDepth + layerDepth;

	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
	return finalTexCoords;
} 


vec2 ParallaxMappingBasic(vec2 texCoords, vec3 viewDir){
	float height =  texture(texture_displacement0, texCoords).r;    
	vec2 p = viewDir.xy / viewDir.z * (height * height_scale);
	return texCoords - p; 
}

void main()
{    
	vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
	vec2 coordinates = vs_textureCoordinates;
	if(displacementMapPresent == 1){
		coordinates = ParallaxMapping(TexCoords,  viewDir);
		if(coordinates.x > 1.0 || coordinates.y > 1.0 || coordinates.x < -1.0 || coordinates.y < -1.0){
			discard;
		}
	}


	//replace coordinates with TexCoords to get back to non parralax mapping
    FragColor = texture(texture_diffuse0, coordinates);

	specularData = texture(texture_specular0, coordinates);
	if(normalMapPresent == 1){
		vec3 normal = texture(texture_normal0, coordinates).rgb;
		normal = normal * 2.0 - 1.0;
		normalData = vec4(normalize(normal),1.0);
	}
	else{
		normalData = vec4(vs_normalData, 1.0);
	}

	positionData = vec4(vs_pos, 1.0);

	eyePositionData = vec4(eyePos,1.0);
	eyeNormalData = vec4(eyeNormal,1.0);
}

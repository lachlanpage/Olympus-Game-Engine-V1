#version 430 core 

in vec2 UV;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 specular;


uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D positionTexture;

uniform float textureSelector;

uniform vec3 cameraPosition;

uniform vec3 Position;
uniform vec3 Color;
uniform float Radius;

void main(){

	//vec3 Position = vec3(2,3,0);
	//vec3 Color = vec3(0.0,1.0,0.0);

	float Linear = 0.7; 
	float Quadratic = 1.8; 

	vec3 FragPos = texture(positionTexture, UV).rgb;
	vec3 Normal = texture(normalTexture, UV).rgb;
	vec3 Diffuse = texture(colorTexture, UV).rgb;
	float Specular = texture(colorTexture, UV).a;

	//calculate lighting like usual 
	vec3 lighting = Diffuse; //hard coded ambient 
	vec3 viewDir = normalize(cameraPosition - FragPos);

	//calc distance between light source and current fragment 
	float distance = length(Position - FragPos);
	if(distance < Radius){
		vec3 lightDir = normalize(Position - FragPos);
		vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * Color; 
		//specular 
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(Normal, halfwayDir),0.0), 16.0);
		vec3 specular = Color * spec * Specular;
		//attenuation
		float attenuation = 1.0 / (1.0 + Linear * distance + Quadratic * distance * distance);
		diffuse *= attenuation;
		specular *= attenuation;
		lighting += diffuse + specular;
	}
	color = vec4(lighting, 1.0);
}


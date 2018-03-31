#version 430 core 
//this is for quad that renders final stuff 
in vec2 UV;
in vec4 shadowCoord;

layout (location = 0) out vec4 color;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D positionTexture;
uniform sampler2D lightTexture;
uniform sampler2D shadowTexture;
uniform sampler2D specularTexture;
uniform sampler2D ssaoTexture;
uniform sampler2D ssaoTextureBlur;
uniform sampler2D albedoTexture; 
uniform sampler2D metallicRoughnessAO; 
uniform sampler2D reflectionTexture;


uniform float textureSelector;

uniform vec3 cameraPosition;
uniform mat4 view;

uniform float gamma;
uniform float exposure;

void main(){
	if(textureSelector == 0){
		color =texture(colorTexture, UV);
	} 
	
	if(textureSelector == 1){
		color =texture(normalTexture, UV);
	}

	if(textureSelector == 2){
		color =texture(positionTexture, UV);
	}

	if(textureSelector == 5){
		//reinhard tone mapping with gamma correction and exposure selector
		//ambient line 
		float ambientOcclusion = texture(ssaoTexture, UV).r; 
		//vec4 hdrCol = texture(lightTexture, UV) + 0.1 * texture(colorTexture, UV);
		//vec4 hdrCol = texture(lightTexture, UV);// + 0.3*ambientOcclusion*texture(colorTexture, UV);
		vec4 hdrCol = texture(lightTexture, UV) + 0.5*texture(colorTexture, UV) + 2*texture(reflectionTexture, UV);
		//exposure tone mapping
		vec3 mapped = (vec4(1.0,1.0,1.0,1.0) - exp(-hdrCol * exposure)).rgb;
		mapped = pow(mapped, vec3(1.0 / gamma));
		color = vec4(mapped,1.0);

		//vec4 colorBeforeGamma = texture(lightTexture, UV) + 0.2 * texture(colorTexture, UV); //hardcoded ambience
		//float gamma = 1.2;
		//color.rgb = pow(colorBeforeGamma.rgb, vec3(1.0/gamma));
		//color.w = 1;
		//color = colorBeforeGamma;
	}

	if(textureSelector == 3){
		//defferred rendering test with fixed lights
		vec3 Position = vec3(0,3,0);
		vec3 Color = vec3(1.0,0.0,0.0);

		float Linear = 0.7; 
		float Quadratic = 1.8; 
		float Radius = 15;

		vec3 FragPos = texture(positionTexture, UV).rgb;
		vec3 Normal = texture(normalTexture, UV).rgb;
		vec3 Diffuse = texture(colorTexture, UV).rgb;
		float Specular = texture(colorTexture, UV).a;

		//calculate lighting like usual 
		vec3 lighting = Diffuse * 0.12; //hard coded ambient 
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

	if(textureSelector == 4){
		//defferred rendering test with fixed lights

		vec3 Position = vec3(2,3,0);
		vec3 Color = vec3(0.0,1.0,0.0);

		float Linear = 0.7; 
		float Quadratic = 1.8; 
		float Radius = 15;

		vec3 FragPos = texture(positionTexture, UV).rgb;
		vec3 Normal = texture(normalTexture, UV).rgb;
		vec3 Diffuse = texture(colorTexture, UV).rgb;
		float Specular = texture(colorTexture, UV).a;

		//calculate lighting like usual 
		vec3 lighting = Diffuse * 0.12; //hard coded ambient 
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

	if(textureSelector == 6){
		color =texture(shadowTexture, UV);
	}

	if(textureSelector == 7){
		color = texture(specularTexture, UV);
	}

	if(textureSelector == 8){
		color = texture(ssaoTexture, UV);
	}

	if(textureSelector == 9){
		color = texture(ssaoTextureBlur, UV);
	}

	if(textureSelector == 10){
		color = texture(albedoTexture, UV);
	}

	if(textureSelector == 11){
		color = texture(metallicRoughnessAO, UV);
	}


}


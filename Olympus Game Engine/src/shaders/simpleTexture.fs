#version 430 core 

in vec2 UV;

layout (location = 0) out vec4 color;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D positionTexture;

uniform float textureSelector;

uniform vec3 cameraPosition;

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

	if(textureSelector == 3){
		//defferred rendering test with fixed lights

		float lightRadius = 10;
		vec3 lightPosition = vec3(12,20,12);
		vec3 lightColor = vec3(1.0,0.0,0.0);

		vec4 image = texture2D(colorTexture, UV);
		vec4 position = texture2D(positionTexture, UV);
		vec4 normal = texture2D(normalTexture, UV);

		vec3 ambient = 0.4* image.rgb;

		vec3 light = vec3(12,20,12);
		vec3 lightDir = light-position.xyz;
		float distance = length(normalize(lightDir));
		float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));   

		normal = normalize(normal);
		lightDir = normalize(lightDir);

		vec3 eyeDir = normalize(cameraPosition - position.xyz);
		vec3 vHalfVector = normalize(lightDir.xyz+eyeDir);

		color = attenuation * max(dot(normal,vec4(lightDir,0.0)),0) * image + attenuation * pow(max(dot(normal,vec4(vHalfVector,0.0)),0.0),100) * 1.5;

	}
}


#version 430 core 


in vec4 vs_pos;

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

in vec4 fsPos;

void main(){
	vec2 UV = (vs_pos.xy / vs_pos.w) * 0.5 + 0.5; //transform from clip space [-1,1] to texture space [0,1]

	color = texture(colorTexture, UV);

	vec3 albedo = texture(colorTexture,UV).xyz;
	vec3 normal = normalize(texture(normalTexture, UV).xyz);
	vec3 pos = texture(positionTexture, UV).xyz;

	vec3 lightToPositionVector = pos.xyz - Position;
	float lightDist = length(lightToPositionVector);
	vec3 l = -lightToPositionVector / (lightDist);

	//fake z test 
	float ztest = step(0.0, Radius-lightDist);

	//light attenuation 
	float d = lightDist / Radius; 
	float attenuation = 1.0 - d; 
	vec3 v = normalize(Position - pos); 
	vec3 h = normalize(1+v);

	vec3 temp = vec3(1.0);

	vec3 color2 = Color * albedo.xyz * max(0.0, dot(normal.xyz,temp)) + Color * 0.4 * pow(max(0.0, dot(h,normal)), 12.0);
	color2 *= ztest * attenuation;

	color2 *= ztest * attenuation;
	color = vec4(color2, 1.0);

	//vec3 lightToPosVector
	//color = vec4(1.0,1.0,1.0, 1.0);
}


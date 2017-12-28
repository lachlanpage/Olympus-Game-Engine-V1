#version 430 core

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D positionTexture;

out vec4 outputColor;

in vec4 vs_pos;
in mat4 inverseProjView;

uniform float lightRadius;
uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform vec3 cameraPosition;

void main() {

	vec2 uv = (vs_pos.xy / vs_pos.w) * 0.5 + 0.5;
	vec3 FragPos = texture(positionTexture, uv).rgb;
	vec3 Normal = texture(normalTexture, uv).rgb;
	vec3 Albedo = texture(colorTexture, uv).rgb;

	//lighting calcs


	//textures 
	vec3 albedo = texture(colorTexture, uv).xyz;
	vec3 n = normalize(texture(normalTexture, uv).xyz);
	vec3 pos = texture(positionTexture, uv).xyz;

	vec3 lightToPositionVector = pos.xyz - lightPosition;
	float lightDistance = length(lightToPositionVector);
	vec3 l = -lightToPositionVector / (lightDistance);

	//fake z test 
	float ztest = step(0.0, lightRadius - lightDistance);

	float d = lightDistance / lightRadius;
	float attenuation = 1.0 - d;
	vec3 v = normalize(cameraPosition - pos);
	vec3 h = normalize(1+v);

	//diffuse and specular
	vec3 color = lightColor * albedo.xyz * max(0.0, dot(n.xyz, vec3(1,1,1))) + lightColor * 0.4 * pow(max(0.0, dot(h,n)), 12.0);

	//z test and attenuation 
	//outputColor *= ztest * attenuation;
	outputColor = vec4(color,1.0);
}

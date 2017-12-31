#version 430 core 

in vec2 UV2;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 specular;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D positionTexture;

uniform vec3 cameraPosition;

void main(){
	vec2 UV = UV2 * 0.5 + 0.5;
	vec3 albedo = texture(colorTexture, UV).xyz;
	vec3 n = normalize(texture(normalTexture, UV).xyz);
	vec3 pos = texture(positionTexture,UV).xyz;

	vec3 l = normalize(vec3(-0.7, 0.3, 0.1));
	vec3 v = normalize(cameraPosition - pos);
	vec3 h = normalize(1+v);

	vec3 col = 0.3 * albedo.xyz * max(0.0, dot(n.xyz, vec3(1.0,1.0,1.0)) + 0.4 * pow(max(0.0, dot(h,n)), 32.0));
	color = vec4(col, 1.0);

}


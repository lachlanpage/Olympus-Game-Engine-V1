#version 430 core 

in vec2 UV2;
in vec4 lightspaceFrag;

layout (location = 0) out vec4 color;


uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D positionTexture;
uniform sampler2D shadowTexture;
uniform sampler2D specularTexture;
uniform sampler2D ssaoTexture;


uniform mat4 lightSpaceMatrix;
uniform vec3 lightDirection;
uniform vec3 cameraPosition;

uniform vec3 lightColor;

float shadowCalculation(vec4 fragPosLightSpace, vec3 norm){
	//perform perspective division as bias matirx not used 
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5 ;//converts to [0,1] from [-1,1] for texture coordinates 
	
	
	//float bias = 0.006;
	float bias = max(0.02 * (1.0 - dot(norm, lightDirection)), 0.006);
	
	//float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  

	//float closestDepth = texture(shadowTexture, projCoords.xy).r;
	float currentDepth = projCoords.z;
	//float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	//if(projCoords.z > 1.0){
	//	shadow = 0.0;
	//}
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowTexture,0);
	for(int x = -1; x <=1; ++x){
		for(int y = -1; y <=1;++y){
			float pcfDepth = texture(shadowTexture, projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 9.0;
	return shadow;
}

void main(){
	vec2 UV = UV2 * 0.5 + 0.5;

	vec3 albedo = texture(colorTexture, UV).xyz;
	vec3 n = normalize(texture(normalTexture, UV).xyz);
	//Fragment position
	vec3 pos = texture(positionTexture,UV).xyz;

	//light direction
	vec3 l = normalize(lightDirection);
	vec3 v = normalize(cameraPosition - pos);
	vec3 h = normalize(l+v);

	vec4 fragPos = lightspaceFrag * vec4(pos,1.0);
	vec4 posFromLight =  lightSpaceMatrix * vec4(pos,1.0); 
	float shadow = shadowCalculation(posFromLight,n);

	float diff= max(dot(n, l), 0.0);
	vec3 diffuse = 0.6 * diff * albedo.xyz;
	
	vec3 viewDir = normalize(cameraPosition - pos.xyz);
	vec3 reflectDir = reflect(-l, n);
	float spec = pow(max(dot(viewDir, reflectDir),0.0), 32);
	vec3 specular = 0.5 * spec * texture(specularTexture, UV).xyz;

	vec3 col = ((1.0 - shadow ) * (diffuse + specular)) * lightColor;

	//vec3 col =  albedo.xyz * max(0.0, dot(n.xyz, l));//) + 0.4 * pow(max(0.0, dot(h,n)), 32.0));
	//vec3 col =  albedo.xyz * max(0.0, dot(n.xyz, vec3(1.0,1.0,1.0)) + 0.4 * pow(max(0.0, dot(h,n)), 32.0));
	//vec3 col = (1.0 - shadow) * albedo.xyz * max(0.0, dot(n.xyz, vec3(1.0,1.0,1.0)) + 0.4 * pow(max(0.0, dot(h,n)), 32.0));
	color = vec4(col, 1.0);

}


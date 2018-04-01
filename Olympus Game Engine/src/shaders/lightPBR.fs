#version 430 core 


in vec4 vs_pos;

layout (location = 0) out vec4 fragColor;


uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D positionTexture;
uniform sampler2D metallicRoughnessAOTexture;


uniform float textureSelector;

uniform vec3 camPos;

uniform vec3 Position;
uniform vec3 lightColor;
uniform float Radius;

in vec4 fsPos;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness){
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

void main(){
	vec2 UV = (vs_pos.xy / vs_pos.w) * 0.5 + 0.5; //transform from clip space [-1,1] to texture space [0,1]

	vec3 albedo = texture(colorTexture,UV).xyz;
	vec3 N = normalize(texture(normalTexture, UV).xyz);
	vec3 pos = texture(positionTexture, UV).xyz;

	float metallic = texture(metallicRoughnessAOTexture, UV).x;
	float roughness = texture(metallicRoughnessAOTexture, UV).y;

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	vec3 lightToPositionVector = pos.xyz - Position;
	float lightDist = length(lightToPositionVector);
	vec3 l = -lightToPositionVector / (lightDist);

	//fake z test 
	float ztest = step(0.0, Radius-lightDist);

	//light attenuation 
	vec3 V = normalize(camPos - pos); 

	//vec3 lightToPosVector
	//color = vec4(1.0,1.0,1.0, 1.0);

	//fake z test 
	// calculate per-light radiance
	vec3 Lo = vec3(0.0);
    vec3 L = normalize(Position - pos);
    vec3 H = normalize(V + L);
    float distance = length(Position - pos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = lightColor * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);   
    float G   = GeometrySmith(N, V, L, roughness);    
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
    vec3 nominator    = NDF * G * F;
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
    vec3 specular = nominator / denominator;
        
        // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;	                
            
    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);        

    // add to outgoing radiance Lo
    Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	
	fragColor = vec4(Lo, 1.0) + vec4(lightColor * albedo.xyz * max(0.0, dot(N.xyz, vec3(1.0,1.0,1.0)))  + lightColor * 0.4 * pow(max(0.0, dot(H,N)), 12.0),1.0);// + lightColor * 0.4 * pow(max(0.0, dot(H,N)), 12.0);
	fragColor *= ztest * attenuation;
	//fragColor = vec4(metallic, 1.0,1.0,1.0);
	//fragColor*= ztest*attenuation;
	//color *= ztest * attenuation;
}

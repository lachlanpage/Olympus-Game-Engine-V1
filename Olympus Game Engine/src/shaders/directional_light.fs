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

vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

float random(vec3 a, int b){
	vec4 seed4 = vec4(a,b);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}

float shadowMapLookup(vec2 coords){
	return texture(shadowTexture, coords).r;
}

float texture2DCompare(sampler2D depths, vec2 uv, float compare){
    float depth = texture2D(depths, uv).r;
    return step(compare, depth);
}

float texture2DShadowLerp(sampler2D depths, vec2 size, vec2 uv, float compare){
    vec2 texelSize = vec2(1.0)/size;
    vec2 f = fract(uv*size+0.5);
    vec2 centroidUV = floor(uv*size+0.5)/size;

    float lb = texture2DCompare(depths, centroidUV+texelSize*vec2(0.0, 0.0), compare);
    float lt = texture2DCompare(depths, centroidUV+texelSize*vec2(0.0, 1.0), compare);
    float rb = texture2DCompare(depths, centroidUV+texelSize*vec2(1.0, 0.0), compare);
    float rt = texture2DCompare(depths, centroidUV+texelSize*vec2(1.0, 1.0), compare);
    float a = mix(lb, lt, f.y);
    float b = mix(rb, rt, f.y);
    float c = mix(a, b, f.x);
    return c;
}

float PCF(sampler2D depths, vec2 size, vec2 uv, float compare){
    float result = 0.0;
    for(int x=-1; x<=1; x++){
        for(int y=-1; y<=1; y++){
            vec2 off = vec2(x,y)/size;
            result += texture2DShadowLerp(depths, size, uv+off, compare);
        }
    }
    return result/9.0;
}

float shadowCalculation(vec4 fragPosLightSpace, vec3 norm){
	//perform perspective division as bias matirx not used 
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5 ;//converts to [0,1] from [-1,1] for texture coordinates 
	
	float bias = max(0.02 * (1.0 - dot(norm, lightDirection)), 0.006);
	float currentDepth = projCoords.z;
	float shadow = 1.0;
	vec2 texelSize = 1.0/ textureSize(shadowTexture,0);
	for(int x = -1; x <=1; ++x){
		for(int y = -1; y <=1;++y){
			float pcfDepth = shadowMapLookup(projCoords.xy + vec2(x,y) * texelSize);
			//float pcfDepth = texture(shadowTexture, projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	
	//vec2 pixelPos = projCoords.xy/texelSize.x + vec2(0.5);
	//vec2 fracPart = fract(pixelPos);
	//vec2 startTexel = (pixelPos - fracPart) * texelSize.x;
		
	//float blTexel = shadowMapLookup(startTexel);
	//float brTexel = shadowMapLookup(startTexel + vec2(texelSize.x , 0.0));
	//float tlTexel = shadowMapLookup(startTexel + vec2(0.0, texelSize.y));
	//float trTexel = shadowMapLookup(startTexel + vec2(texelSize.x, texelSize.y));

	//float mixA = mix(blTexel, tlTexel, fracPart.y);
	//float mixB = mix(brTexel, trTexel, fracPart.y);

	//float mixFinal = mix(mixA, mixB, fracPart.x);

	//float mixFinal2 = mix(mixFinal, shadow/9.0, 0.8);
	//float mixB = mix()

	//int index = int(16.0 * random(gl_FragCoord.xyy, 2))%16;

	//}
	//return 1-mixFinal;
	shadow /= 9.0;
	return shadow;
	//return mixFinal2;
	//return 1-shadow;
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

	color = vec4(col, 1.0);

}


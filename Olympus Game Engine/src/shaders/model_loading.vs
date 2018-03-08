#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 biTangent;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

out vec2 vs_textureCoordinates;
out vec3 vs_normalData;
out vec3 vs_pos;

out vec3 tangentViewPos;
out vec3 tangentFragPos;

out vec3 eyePos;
out vec3 eyeNormal;
out mat3 TBN;

out vec3 aTan;
out vec3 aBitan;
out vec3 aNorm;

mat3 calculate_TBN(vec3 tangent, vec3 bitangent, vec3 normal){
	vec3 T = normalize(vec3(model* vec4(tangent,0.0)));
	vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(normal, 0.0)));
	mat3 TBN = transpose(mat3(T,B,N));

	aTan = T;
	aBitan = B;
	aNorm = N;
	return TBN;
}

void main()
{
	mat3 TBN = calculate_TBN(aTangent, biTangent, aNormal);
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_normalData = aNormal;
	eyeNormal = (view * model * vec4(aNormal,1.0)).xyz;
	eyePos = vec3(view*model*vec4(aPos,1.0));
	vs_textureCoordinates = aTexCoords;
	vs_pos = vec3(model*vec4(aPos,1.0));

	tangentViewPos = TBN * viewPos;
	tangentFragPos = TBN * vec3(model*vec4(aPos, 1.0));
}

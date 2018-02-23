#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 vs_textureCoordinates;
out vec3 vs_normalData;
out vec3 vs_pos;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);

	vs_normalData = (view*model*vec4(aNormal,1.0)).xyz;
	vs_textureCoordinates = aTexCoords;
	vs_pos = vec3(view*model*vec4(aPos,1.0));
}

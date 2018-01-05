#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 textureCoordinates;

out vec2 UV2;
out vec4 lightspaceFrag;

uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = vec4(aPos.x,aPos.y, aPos.z, 1.0);
	UV2 = vec2(aPos.x,aPos.y);
	lightspaceFrag = lightSpaceMatrix * vec4(aPos.x,aPos.y,aPos.z,1.0);
}
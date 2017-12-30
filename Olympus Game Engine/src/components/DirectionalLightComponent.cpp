#include "DirectionalLightComponent.h"

DirectionalLightComponent::DirectionalLightComponent() {
	m_shader = ResourceManager::Instance()->loadShader("src/shaders/directional_light.vs", "src/shaders/directional_light.fs");

	//Create the appropriate buffers for the cube
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(textureCoordinates), NULL, GL_STATIC_DRAW);
	//attributes for shader
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	//offset for texture coordinates (location = 1), normalData location = 2
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(textureCoordinates), textureCoordinates);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) sizeof(vertices));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}
void DirectionalLightComponent::update(Entity& entity) {
	m_shader->use();
	m_shader->setVec3("cameraPosition", Camera::Instance()->getPosition());
	getBufferTextures();
	glBindVertexArray(VAO);
	sendToRenderer(GL_TRIANGLES, 0, 6);
}

void DirectionalLightComponent::postInit(Entity& entity) {

}

void DirectionalLightComponent::getBufferTextures() {
	Renderer::Instance()->getBufferTextures(m_shader);
}

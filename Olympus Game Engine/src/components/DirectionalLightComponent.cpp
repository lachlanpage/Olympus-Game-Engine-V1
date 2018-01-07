#include "DirectionalLightComponent.h"

DirectionalLightComponent::DirectionalLightComponent(glm::vec3 direction) {
	m_shader = ResourceManager::Instance()->loadShader("src/shaders/directional_light.vs", "src/shaders/directional_light.fs");
	m_direction = direction;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(textureCoordinates), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(textureCoordinates), textureCoordinates);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) sizeof(vertices));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}
void DirectionalLightComponent::update(Entity& entity) {
	//update light direction in settings which will affect shadows 
	//at the moment want directional light to rotate direction over time for testing hence the cos() for m_direction.x
	m_direction = glm::vec3(cos(SDL_GetTicks() / 10000.0), m_direction.y, m_direction.z);
	Settings::Instance()->setLightDirection(m_direction);

	m_shader->use();
	m_shader->setVec3("cameraPosition", Camera::Instance()->getPosition());
	m_shader->setMat4("lightSpaceMatrix", Settings::Instance()->lightSpaceMatrix);
	m_shader->setVec3("lightDirection", m_direction);
	getBufferTextures();
	glBindVertexArray(VAO);
	sendToRenderer(GL_TRIANGLES, 0, 6);
}

void DirectionalLightComponent::postInit(Entity& entity) {

}

void DirectionalLightComponent::getBufferTextures() {
	Renderer::Instance()->getBufferTextures(m_shader);
}

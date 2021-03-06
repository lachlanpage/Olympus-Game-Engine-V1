#include "QuadGraphicsComponent.h"

QuadGraphicsComponent::QuadGraphicsComponent() {
	m_shader = ResourceManager::Instance()->loadShader("src/shaders/passthrough.vs", "src/shaders/simpleTexture.fs");

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
void QuadGraphicsComponent::update(Entity& entity) {
	m_shader->use();
	m_shader->setMat4("view", Settings::Instance()->depthViewMatrix);
	m_shader->setMat4("projection", Settings::Instance()->projectionMatrix);
	glm::mat4 model;
	model = glm::translate(model, entity.getPosition());
	m_shader->setMat4("model", model);
	glBindVertexArray(VAO);
	updateQuadShader(m_shader);
	//sendToRenderer(GL_TRIANGLES, 0, 6);
}


void QuadGraphicsComponent::updateQuadShader(Shader * shader) {
	Renderer::Instance()->updateQuadShader(shader);
}

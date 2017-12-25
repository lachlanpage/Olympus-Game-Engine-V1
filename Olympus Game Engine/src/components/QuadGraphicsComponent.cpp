#include "QuadGraphicsComponent.h"

QuadGraphicsComponent::QuadGraphicsComponent() {
	m_shader = ResourceManager::Instance()->loadShader("src/shaders/cube.vs", "src/shaders/cube.fs");

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
void QuadGraphicsComponent::update(Entity& entity) {
	m_shader->use();
	m_shader->setMat4("view", Camera::Instance()->getViewMatrix());
	m_shader->setMat4("projection", Settings::Instance()->projection);
	glm::mat4 model;
	model = glm::translate(model, entity.getPosition());
	m_shader->setMat4("model", model);
	glBindVertexArray(VAO);
	sendToRenderer(GL_TRIANGLES, 0, 6);
}

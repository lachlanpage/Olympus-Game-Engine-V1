#include "CubeGraphicsComponent.h"

CubeGraphicsComponent::CubeGraphicsComponent() {
	m_shader = ResourceManager::Instance()->loadShader("src/shaders/basic.vs", "src/shaders/basic.fs");

	//Create the appropriate buffers for the cube
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(textureCoordinates) + sizeof(normalData), NULL, GL_STATIC_DRAW);
	//attributes for shader
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	//offset for texture coordinates (location = 1), normalData location = 2
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(textureCoordinates), textureCoordinates);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(textureCoordinates), sizeof(normalData), normalData);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) sizeof(vertices));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(vertices) + sizeof(textureCoordinates)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);



}
void CubeGraphicsComponent::update(Entity& entity) {
	m_shader->use();
	m_shader->setMat4("view", Camera::Instance()->getViewMatrix());
	m_shader->setMat4("projection", Settings::Instance()->projection);
	glm::mat4 model;
	model = glm::translate(model, entity.getPosition());
	m_shader->setMat4("model", model);
	glBindVertexArray(cubeVAO);
	sendToRenderer(GL_TRIANGLES, 0, 36);


}

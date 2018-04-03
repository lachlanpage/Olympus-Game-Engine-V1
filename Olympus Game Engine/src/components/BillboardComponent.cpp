#include "BillboardComponent.h"

BillboardComponent::BillboardComponent() {
	// setup plane VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	m_shader = ResourceManager::Instance()->loadShader("src/shaders/billboard.vs", "src/shaders/billboard.fs");
	grassTexture = ResourceManager::Instance()->loadTexture("textures/tree.png");
	
	ResourceManager::Instance()->printTextures();

	positionList.push_back(glm::vec3(0, 0, 0));
	positionList.push_back(glm::vec3(0, 10, 0));
}

void BillboardComponent::update(Entity& entity){
	glm::mat4 view = Camera::Instance()->getViewMatrix();
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_shader->use();
	m_shader->setMat4("projection", Settings::Instance()->projection);
	m_shader->setInt("grassTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassTexture);

	glm::mat4 model;
	model = glm::translate(model, entity.getPosition());
	
	glm::mat4 modelView = Camera::Instance()->getViewMatrix()*model;

	modelView[0][0] = 1;
	modelView[0][1] = 0;
	modelView[0][2] = 0;

	modelView[2][0] = 0;
	modelView[2][1] = 0;
	modelView[2][2] = 1;
	//model = glm::rotate(model, particle.getRotation(), glm::vec3(0, 0, 1));

	//m_shader->setMat4("model", model);
	m_shader->setMat4("viewModel",modelView);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glDepthMask(true);
	glDisable(GL_BLEND);
	
}

void BillboardComponent::renderShadow(Entity& entity){}
void BillboardComponent::postInit(Entity& entity){}


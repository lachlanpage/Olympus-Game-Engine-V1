#include "PointLightGraphicsComponent.h"

PointLightGraphicsComponent::PointLightGraphicsComponent(){

	m_shader = ResourceManager::Instance()->loadShader("src/shaders/pointLight.vs", "src/shaders/pointLight.fs");
	m_color = glm::vec3(1.0, 0.0, 0.0);
	m_stacks = 40;
	m_slices = 40;
	//generate sphere vertices
	int radius = 10;
	for (int stack = 0; stack < 40; ++stack) {
		for (int slice = 0; slice < m_slices; ++slice) {
			float y = 2.0 * stack / m_stacks - 1.0;
			float r = sqrt(1 - y*y);
			float x = r * sin(2.0 * 3.1415 * slice / m_slices);
			float z = r * cos(2.0 * 3.1415 * slice / m_slices);

			vertices.push_back(radius*x);
			vertices.push_back(radius*y);
			vertices.push_back(radius*z);
		}
	}

	//Create the appropriate buffers for the cube
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

}
void PointLightGraphicsComponent::update(Entity& entity) {


	m_shader->use();

	//lightradius, lightpoisiton, lightcolor 

	m_shader->setMat4("view", Camera::Instance()->getViewMatrix());
	m_shader->setMat4("projection", Settings::Instance()->projection);
	glm::mat4 model;
	model = glm::translate(model, entity.getPosition());
	m_shader->setMat4("model", model);
	m_shader->setVec3("lightPosition", entity.getPosition());
	m_shader->setFloat("lightRadius", 10);
	m_shader->setVec3("lightColor", m_color);
	glBindVertexArray(VAO);
	Renderer::Instance()->updateLightShader(m_shader);
	sendToRenderer(GL_TRIANGLES, 0, m_stacks*m_slices);
}

void PointLightGraphicsComponent::setLightColor(glm::vec3 color) {
	m_color = color;
}

glm::vec3 PointLightGraphicsComponent::getLightColor() {
	return m_color;
}
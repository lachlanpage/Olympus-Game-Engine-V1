#include "LightComponent.h"
#include <iostream>
LightComponent::LightComponent(float radius, glm::vec3 color) {
	std::cout << radius << std::endl;
	m_radius = radius;
	m_color = color;

	m_shader = ResourceManager::Instance()->loadShader("src/shaders/light.vs", "src/shaders/light.fs");

	m_stacks = 40;
	m_slices = 40;
	int i, j;
	int lats = 40;
	int longs = 40;
	int indicator = 0;
	for (i = 0; i <= lats; i++) {
		double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 =  cos(lat0);

		double lat1 = glm::pi<double>() * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		for (j = 0; j <= longs; j++) {
			double lng = 2 * glm::pi<double>() * (double)(j - 1) / longs;
			double x =  cos(lng);
			double y =  sin(lng);

			vertices.push_back(x * zr0 *m_radius);
			vertices.push_back(y * zr0*m_radius);
			vertices.push_back(z0*m_radius);
			indices.push_back(indicator);
			indicator++;

			vertices.push_back(x * zr1*m_radius);
			vertices.push_back(y * zr1*m_radius);
			vertices.push_back(z1*m_radius);
			indices.push_back(indicator);
			indicator++;
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

	glGenBuffers(1, &m_vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

void LightComponent::update(Entity& entity) {
	m_shader->use();
	m_shader->setMat4("view", Camera::Instance()->getViewMatrix());
	m_shader->setMat4("projection", Settings::Instance()->projection);
	glm::mat4 model;
	model = glm::translate(model, entity.getPosition());
	m_shader->setMat4("model", model);
	m_shader->setVec3("Position", entity.getPosition());
	m_shader->setFloat("Radius", m_radius);
	m_shader->setVec3("Color", m_color);
	updateLightShader();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
	glDrawElements(GL_QUAD_STRIP, indices.size(), GL_UNSIGNED_INT, NULL);
	//sendToRenderer(GL_TRIANGLES, 0, 40 * 40*3);
}

void LightComponent::updateLightShader() {
	Renderer::Instance()->updateLightShader(m_shader);
}

void LightComponent::postInit(Entity& entity) {

}


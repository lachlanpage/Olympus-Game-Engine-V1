#include "SphereGraphicsComponent.h"


SphereGraphicsComponent::SphereGraphicsComponent(float metallic, float roughness) {
	m_shader = ResourceManager::Instance()->loadShader("src/shaders/pbr.vs", "src/shaders/pbr.fs");
	m_shadowShader = ResourceManager::Instance()->loadShader("src/shaders/shadow.vs", "src/shaders/shadow.fs");

	//std::cout << metallic << " " << roughness << std::endl;
	m_roughness = roughness;
	m_metallic = metallic;

	//Create the appropriate buffers for the cube
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359;
	for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.push_back(glm::vec3(xPos, yPos, zPos));
			uv.push_back(glm::vec2(xSegment, ySegment));
			normals.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	bool oddRow = false;
	for (int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (int x = 0; x <= X_SEGMENTS; ++x)
			{
				indices.push_back(y       * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y       * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
	indexCount = indices.size();

	std::vector<float> data;
	for (int i = 0; i < positions.size(); ++i)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);
		if (uv.size() > 0)
		{
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}
		if (normals.size() > 0)
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	float stride = (3 + 2 + 3) * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);


	//set required pbr textures for IBL 
	irradianceMap = ResourceManager::Instance()->loadTexture("irradianceMap");
	prefilterMap = ResourceManager::Instance()->loadTexture("prefilterMap");
	brdfMap = ResourceManager::Instance()->loadTexture("brdfTexture");
}

void SphereGraphicsComponent::update(Entity& entity) {

	glm::vec3 lightPositions[] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};

	std::cout << Time::Instance()->getRuntimeMilliseconds() << std::endl;
	
	float xrot = cos(0.001)*(entity.getPosition().x - 0) - sin(0.001)*(entity.getPosition().y - 0) + 0;
	float yrot = sin(0.001)*(entity.getPosition().x - 0) + cos(0.001)*(entity.getPosition().y - 0) + 0;
	entity.setPosition(glm::vec3(xrot, yrot, entity.getPosition().z));

	m_shader->use();
	m_shader->setMat4("view", Camera::Instance()->getViewMatrix());
	m_shader->setMat4("projection", Settings::Instance()->projection);
	glm::mat4 model;
	model = glm::translate(model, entity.getPosition());
	m_shader->setMat4("model", model);
	m_shader->setFloat("m_metallic", m_metallic);
	m_shader->setFloat("m_roughness", m_roughness);
	m_shader->setVec3("camPos", Camera::Instance()->getPosition());

	m_shader->setVec3("lightPositions[0]", lightPositions[0]);
	m_shader->setVec3("lightPositions[1]", lightPositions[1]);
	m_shader->setVec3("lightPositions[2]", lightPositions[2]);
	m_shader->setVec3("lightPositions[3]", lightPositions[3]);


	m_shader->setInt("irradianceMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);

	m_shader->setInt("prefilterMap", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);

	m_shader->setInt("brdfLUT", 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, brdfMap);
	

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

void SphereGraphicsComponent::renderShadow(Entity& entity){

	m_shadowShader->use();
	m_shadowShader->setMat4("view", Settings::Instance()->depthViewMatrix);
	m_shadowShader->setMat4("projection", Settings::Instance()->projectionMatrix);
	glm::mat4 model;
	model = glm::translate(model, entity.getPosition());
	model = glm::scale(model, entity.getScale());
	//x rotation
	model = glm::rotate(model, glm::radians(entity.getRotation().x), glm::vec3(1, 0, 0));
	//y rotation 
	model = glm::rotate(model, glm::radians(entity.getRotation().y), glm::vec3(0, 1, 0));
	//z rotation
	model = glm::rotate(model, glm::radians(entity.getRotation().z), glm::vec3(0, 0, 1));
	m_shadowShader->setMat4("model", model);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);



}
void SphereGraphicsComponent::postInit(Entity& entity){}


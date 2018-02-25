#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(){
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

	srand(time(NULL));

	particleShader = new Shader("src/shaders/particle.vs", "src/shaders/particle.fs");

	//ParticlesContainer.push_back(Particle(glm::vec3(5,7,1), glm::vec3(0,30,0), -10,10,0,1));
	//ParticlesContainer.push_back(Particle(glm::vec3(5, 6, 1), glm::vec3(0, 1, 0), 10, 10, 0, 0));
	//ParticlesContainer.push_back(Particle(glm::vec3(3, 6, 1), glm::vec3(0, 1, 0), 10, 10, 0, 0));
	//ParticlesContainer.push_back(Particle(glm::vec3(2, 6, 1), glm::vec3(0, 1, 0), 10, 20, 0, 0));
	//ParticlesContainer.push_back(Particle(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), 10, 20, 0, 0));

	//75fps with 10,000 particles
	MaxParticles = 100;
	rowsParticleTexture = 8;

	particleTexture = ResourceManager::Instance()->loadTexture("textures/fire.png");
}
void ParticleGenerator::update(Entity& entity){
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(false);
	particleShader->use();
	particleShader->setMat4("view", Camera::Instance()->getViewMatrix());
	particleShader->setMat4("projection", Settings::Instance()->projection);
	glm::mat4 view = Camera::Instance()->getViewMatrix();

	particleShader->setInt("particleTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, particleTexture);

	for (auto particle : ParticlesContainer) {
		particle.updateTextureData(rowsParticleTexture);
		glm::mat4 model;
		model = glm::translate(model, particle.getPosition());
		//model = glm::scale(model, particle.getScale());
		model[0][0] = view[0][0];
		model[0][1] = view[1][0];
		model[0][2] = view[2][0];
		model[1][0] = view[0][1];
		model[1][1] = view[1][1];
		model[1][2] = view[2][1];
		model[2][0] = view[0][2];
		model[2][1] = view[1][2];
		model[2][2] = view[2][2];
		model = glm::scale(model, entity.getScale()+particle.getScale());
		model = glm::rotate(model, particle.getRotation(), glm::vec3(0, 0, 1));
		particleShader->setMat4("model", model);

		particleShader->setVec2("texOffset1", particle.texOffset1);
		particleShader->setVec2("texOffset2", particle.texOffset2);
		particleShader->setVec2("texCoordInfo", glm::vec2(rowsParticleTexture, particle.getBlend()));

		

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	

	glBindVertexArray(0);

	glDepthMask(true);
	glDisable(GL_BLEND);

	//update dead particles 
	for (int i = 0; i < ParticlesContainer.size(); i++) {
		bool stillAlive = ParticlesContainer[i].update();
		if(!stillAlive)
			ParticlesContainer.erase(ParticlesContainer.begin() + i);
	}

	//create new particles
	float particlesToCreate = Time::Instance()->getDeltaTime() * 500;
	int count = (int)floor(particlesToCreate);

	for (int i = 0; i < count; i++) {
		emitParticle(entity);
	}

	//sort particles
	ParticlesContainer = InsertionSort(ParticlesContainer);

}


std::vector<Particle> ParticleGenerator::InsertionSort(std::vector<Particle> copyContainer) {
	int j;
	Particle temp;
	for (int i = 0; i < copyContainer.size(); i++) {
		j = i;
		while (j > 0 && copyContainer[j].getDistance() < copyContainer[j - 1].getDistance()) {
			std::iter_swap(copyContainer.begin() + j, copyContainer.begin() + j - 1);
			j--;
		}
	}
	return copyContainer;
}

void ParticleGenerator::emitParticle(Entity& entity) {
	float dirX = rand() % 12 - 6;
	float dirZ = rand() % 12 - 6;
	glm::vec3 velocity = glm::vec3(dirX, 1, dirZ);

	Particle aParticle = Particle(entity.getPosition(), velocity, -0.1, rand() % 3 + 1, rand() % 100 + 1, rand() % 8 + 1);
	ParticlesContainer.push_back(aParticle);
}
void ParticleGenerator::renderShadow(Entity& entity){}
void ParticleGenerator::postInit(Entity& entity){}
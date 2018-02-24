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

	ParticlesContainer.push_back(Particle(glm::vec3(5,7,1), glm::vec3(0,30,0), -10,10,0,1));
	ParticlesContainer.push_back(Particle(glm::vec3(5, 6, 1), glm::vec3(0, 1, 0), 10, 10, 0, 0));
	ParticlesContainer.push_back(Particle(glm::vec3(3, 6, 1), glm::vec3(0, 1, 0), 10, 10, 0, 0));
	ParticlesContainer.push_back(Particle(glm::vec3(2, 6, 1), glm::vec3(0, 1, 0), 10, 20, 0, 0));
	ParticlesContainer.push_back(Particle(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), 10, 20, 0, 0));

	MaxParticles = 100;
}
void ParticleGenerator::update(Entity& entity){
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false);
	particleShader->use();
	particleShader->setMat4("view", Camera::Instance()->getViewMatrix());
	particleShader->setMat4("projection", Settings::Instance()->projection);

	glm::mat4 view = Camera::Instance()->getViewMatrix();

	for (auto particle : ParticlesContainer) {
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
		model = glm::scale(model, entity.getScale());
		particleShader->setMat4("model", model);

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
	std::cout << ParticlesContainer.size() << std::endl;
	while (ParticlesContainer.size() <= MaxParticles) {
		//Particle aParticle = Particle(glm::vec3(entity.getPosition().x + rand() % 1 , entity.getPosition().y + rand() % 1, entity.getPosition().z + rand() % 1 ),
		//	glm::vec3(rand() % 10, rand() % 10, rand() % 10), -1, rand() % 20 + 4, rand() % 100 + 1, 0);
		Particle aParticle = Particle(glm::vec3(entity.getPosition()), glm::vec3(rand()%30+1 - 15, rand() % 30 + 1 - 15, rand() % 30 + 1 - 15), 1, rand()%10+1, 0, 1);
		ParticlesContainer.push_back(aParticle);
	}
	//for (auto particle : ParticlesContainer) {
	//	bool stillAlive = particle.update();
	//	if (!stillAlive) {
	//
	//	}
	//}


}
void ParticleGenerator::renderShadow(Entity& entity){}
void ParticleGenerator::postInit(Entity& entity){}
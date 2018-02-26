#include "ParticleGenerator.h"

int MAX_INSTANCES = 10000;
int INSTANCE_DATA_LENGTH = 21;

ParticleGenerator::ParticleGenerator(){
	// setup plane VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	//buffer container vertices
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &particleDataVBO);
	glBindBuffer(GL_ARRAY_BUFFER, particleDataVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * INSTANCE_DATA_LENGTH * MAX_INSTANCES, NULL, GL_STREAM_DRAW);
	glBindVertexArray(quadVAO);
	//col 1 -> 4 of matrix
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, INSTANCE_DATA_LENGTH * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);
	
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, INSTANCE_DATA_LENGTH * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);
	
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, INSTANCE_DATA_LENGTH * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);
	
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, INSTANCE_DATA_LENGTH * sizeof(float), (void*)(12 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);
	//texture offsets
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, INSTANCE_DATA_LENGTH * sizeof(float), (void*)(16 * sizeof(float)));
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);
	//blend
	
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, INSTANCE_DATA_LENGTH * sizeof(float), (void*)(20 * sizeof(float)));
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	srand(time(NULL));

	particleShader = new Shader("src/shaders/particle.vs", "src/shaders/particle.fs");

	particleShader->use(); 


	//60 fps with 500 particles per second
	//150 fps with 500 particles per second
	rowsParticleTexture = 2;
	additiveBlending = 1;
	m_pps = 500;
	lifetime = 10;
	gravity = 1;

	velocity = glm::vec3(1, 1, 1);

	particleTexture = ResourceManager::Instance()->loadTexture("textures/wick.png");
}
void ParticleGenerator::update(Entity& entity){
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	if(additiveBlending)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	else
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	particleShader->use();

	glm::mat4 view = Camera::Instance()->getViewMatrix();
	particleShader->setMat4("viewProjection", Settings::Instance()->projection*Camera::Instance()->getViewMatrix());
	particleTexture = ResourceManager::Instance()->loadTexture("textures/wick.png");
	particleShader->setInt("particleTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, particleTexture);
	particleShader->setFloat("numRows", rowsParticleTexture);

	std::vector<float> vboDataVec;

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

		//Position Data 

		//model data //1st col 
		vboDataVec.push_back(model[0][0]);
		vboDataVec.push_back(model[0][1]);
		vboDataVec.push_back(model[0][2]);
		vboDataVec.push_back(model[0][3]);

		vboDataVec.push_back(model[1][0]);
		vboDataVec.push_back(model[1][1]);
		vboDataVec.push_back(model[1][2]);
		vboDataVec.push_back(model[1][3]);

		vboDataVec.push_back(model[2][0]);
		vboDataVec.push_back(model[2][1]);
		vboDataVec.push_back(model[2][2]);
		vboDataVec.push_back(model[2][3]);

		vboDataVec.push_back(model[3][0]);
		vboDataVec.push_back(model[3][1]);
		vboDataVec.push_back(model[3][2]);
		vboDataVec.push_back(model[3][3]);

		vboDataVec.push_back(particle.texOffset1.x);
		vboDataVec.push_back(particle.texOffset1.y);
		vboDataVec.push_back(particle.texOffset2.x);
		vboDataVec.push_back(particle.texOffset2.y);

		vboDataVec.push_back(particle.getBlend());
	}

	
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, particleDataVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vboDataVec.size(), &vboDataVec[0]);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesContainer.size());
	

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
	float particlesToCreate = Time::Instance()->getDeltaTime() * m_pps;
	if (ParticlesContainer.size() < MAX_INSTANCES-10) {
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

	float velX = velocity.x;
	float velY = velocity.y;
	float velZ = velocity.z;
	//float dirX = rand() % velocity.x - 3;
	//float dirZ = rand() % 6 - 3;
	//glm::vec3 velocity = glm::vec3(dirX, 1, dirZ);

	Particle aParticle = Particle(entity.getPosition(), glm::vec3(velocity.x, velocity.y, velocity.z), gravity, rand() % 3 + 1, lifetime, rand() % 10 + 1);
	//Particle aParticle = Particle(entity.getPosition(), glm::vec3(0, 0, 0), 0, 100, 0, 0);
	ParticlesContainer.push_back(aParticle);
}
void ParticleGenerator::renderShadow(Entity& entity){}
void ParticleGenerator::postInit(Entity& entity){}


int  ParticleGenerator::getPPS() { return m_pps; }
void  ParticleGenerator::setPPS(int val) { m_pps = val; }

glm::vec3  ParticleGenerator::getVelocity() { return velocity; }
void  ParticleGenerator::setVelocity(glm::vec3 vel) { velocity = vel; }

float  ParticleGenerator::getLifetime() { return lifetime; }
void  ParticleGenerator::setLifetime(float lifespan) { lifetime = lifespan; }

void ParticleGenerator::setGravity(float grav) {
	gravity = grav;
}

float ParticleGenerator::getGravity() { return gravity; }
#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Component.h"
#include "../core/Shader.h"
#include "Particle.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
class ParticleGenerator : public Component {
public:
	ParticleGenerator();
	virtual void update(Entity& entity);
	void renderShadow(Entity& entity);
	virtual void postInit(Entity& entity);

	std::vector<Particle> ParticlesContainer;
private:
	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;

	GLfloat quadVertices[20] = {
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	Shader* particleShader;

	unsigned int quadVAO;
	unsigned int quadVBO;

	glm::vec3 position;
	glm::vec3 velocity;
	float gravity;
	float lifetime;
	int MaxParticles;

	//particle update 
	//velocity.y ++ gravity

};
#endif
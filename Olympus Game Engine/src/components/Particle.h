#ifndef PARTICLE_H 
#define PARTICLE_H 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../utilities/Time.h"
class Particle {
public:

	Particle(glm::vec3 position, glm::vec3 velocity, float gravity, float lifetime, float rotation, float scale);
	glm::vec3 getPosition();
	float getRotation();
	float getScale();
	
	bool update();

private:
	glm::vec3 position;
	glm::vec3 velocity;
	float gravity;
	float lifetime;
	float rotation;
	float scale;
	float elapsedTime = 0;
};
#endif
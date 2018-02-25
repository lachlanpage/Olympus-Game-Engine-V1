#ifndef PARTICLE_H 
#define PARTICLE_H 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../utilities/Time.h"
#include "../utilities/Camera.h"
class Particle {
public:
	Particle();
	Particle(glm::vec3 position, glm::vec3 velocity, float gravity, float lifetime, float rotation, float scale);
	glm::vec3 getPosition();
	float getRotation();
	float getScale();
	float getBlend();
	
	bool update();
	void updateTextureData();
	float getDistance();
	void setTextureOffset(glm::vec2 offset, int index);

	glm::vec2 texOffset1;
	glm::vec2 texOffset2;

private:
	glm::vec3 position;
	glm::vec3 velocity;

	float distance;

	float gravity;
	float lifetime;
	float rotation;
	float scale;
	float elapsedTime = 0;
	float blend;

};
#endif
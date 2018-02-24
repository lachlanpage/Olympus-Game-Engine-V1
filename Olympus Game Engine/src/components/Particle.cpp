#include "Particle.h"

Particle::Particle(glm::vec3 position, glm::vec3 velocity, float gravity, float lifetime, float rotation, float scale) {
	this->position = position;
	this->velocity = velocity;
	this->gravity = gravity;
	this->lifetime = lifetime;
	this->rotation = rotation;
	this->scale = scale;
}

glm::vec3 Particle::getPosition() { return position; }
float Particle::getRotation() {
	return rotation;
}
float Particle::getScale() {
	return scale;
}

bool Particle::update() {
	velocity.y += -50 * gravity * Time::Instance()->getDeltaTime();
	glm::vec3 change = glm::vec3(velocity.x,velocity.y,velocity.z);
	change = change * glm::vec3(Time::Instance()->getDeltaTime(), Time::Instance()->getDeltaTime(), Time::Instance()->getDeltaTime());
	position = change + position;
	elapsedTime += Time::Instance()->getDeltaTime();
	return elapsedTime < lifetime;
}
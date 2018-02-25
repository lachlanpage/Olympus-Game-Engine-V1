#include "Particle.h"

#include <iostream>
Particle::Particle(glm::vec3 position, glm::vec3 velocity, float gravity, float lifetime, float rotation, float scale) {
	this->position = position;
	this->velocity = velocity;
	this->gravity = gravity;
	this->lifetime = lifetime;
	this->rotation = rotation;
	this->scale = scale;
}

Particle::Particle(){}

glm::vec3 Particle::getPosition() { return position; }
float Particle::getRotation() {
	return rotation;
}
float Particle::getScale() {
	return scale;
}

float Particle::getBlend() {
	return blend;
}

float Particle::getDistance() {
	return distance;
}
bool Particle::update() {
	velocity.y += -50 * gravity * Time::Instance()->getDeltaTime();
	glm::vec3 change = glm::vec3(velocity.x,velocity.y,velocity.z);
	change = change * glm::vec3(Time::Instance()->getDeltaTime(), Time::Instance()->getDeltaTime(), Time::Instance()->getDeltaTime());
	position = change + position;
	elapsedTime += Time::Instance()->getDeltaTime();
	//update camera stuff 
	distance = glm::length(Camera::Instance()->getPosition() - position);
	return elapsedTime < lifetime;
}

void Particle::updateTextureData() {
	float lifeFactor = elapsedTime / lifetime;
	int stageCount = 8 * 8;
	float atlasProgression = lifeFactor * stageCount;

	int index1 = (int) floor(atlasProgression);
	int index2 = index1 < stageCount - 1 ? index1 + 1 : index1;
	blend = std::fmod(atlasProgression, 1.0);

	int col = index1 % 8;
	int row = index1 / 8;
	texOffset1.x = (float)col / 8;
	texOffset1.y = (float)row / 8;

	col = index2 & 8;
	row = index2 / 8;
	texOffset2.x = (float)col / 8;
	texOffset2.y = (float)row / 8;

}


void Particle::setTextureOffset(glm::vec2 offset, int index) {
	int col = index % 8;
	int row = index / 8;
	offset.x = (float)col / 8;
	offset.y = (float)row / 8;
}
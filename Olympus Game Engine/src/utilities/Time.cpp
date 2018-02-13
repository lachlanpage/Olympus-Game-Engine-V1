#include "Time.h"

Time* Time::m_Instance = nullptr;

Time* Time::Instance() {
	if (m_Instance == nullptr)
		m_Instance = new Time;
	return m_Instance;
}

Time::Time() {
	deltaTime = 0;
	lastFrame = 0;
	currentFrame = SDL_GetTicks();
}

void Time::update() {
	currentFrame = SDL_GetTicks();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

float Time::getDeltaTime() {
	//divide by 1000 to convert from miliseconds to seconds
	return deltaTime / 1000; 
}

float Time::getRuntime() {
	return SDL_GetTicks() / 1000;
}

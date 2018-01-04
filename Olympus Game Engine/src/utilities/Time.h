#ifndef TIME_H
#define TIME_H 
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
class Time {
	//singleton class to retrieve the time running.
	//Should be used in update methods of a component/entity
public:
	static Time* Instance();
	float getRuntime();
	float getDeltaTime();
	void update();
private:
	Time();
	static Time* m_Instance;
	float currentFrame;
	float deltaTime;
	float lastFrame;

};

#endif
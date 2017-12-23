#ifndef TIME_H
#define TIME_H 
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
class Time {
public:
	static Time* Instance();
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
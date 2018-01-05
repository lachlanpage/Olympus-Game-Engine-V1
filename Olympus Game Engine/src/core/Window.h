#ifndef WINDOW_H
#define WINDOW_H
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <glad/glad.h>

#include "../utilities/Logger.h"
#include "../utilities/MessageBus.h"
#include "../utilities/Message.h"
#include "../utilities/BusNode.h"
#include "../utilities/Time.h"

class Window : public BusNode {
public:
	Window(const char* title, int width, int height, MessageBus* messageBus);
	~Window();
	bool isRunning();
	void handleInput();
	SDL_Window *getWindow();
private:
	void onNotify(Message message);
	SDL_Window *window;
	SDL_GLContext context;
	bool running;
};
#endif
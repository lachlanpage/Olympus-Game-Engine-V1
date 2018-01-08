#include "Window.h"
Window::Window(const char* title, int width, int height, MessageBus* messageBus) : BusNode(messageBus) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		Logger::Instance()->write("SDL_INIT_VIDEO FAIL");

	window = SDL_CreateWindow(title, 0, 0, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (window == NULL)
		Logger::Instance()->write("SDL_CREATE_WINDOW_ERROR");

	context = SDL_GL_CreateContext(window);
	if (context == NULL)
		Logger::Instance()->write("SDL_CREATE_CONTEXT_ERROR");

	//OPENGL 4.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//vsync: 1 to activate, 0 to disable
	SDL_GL_SetSwapInterval(0);
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	glViewport(0, 0, 800, 600);
	running = true;
}

SDL_Window *Window::getWindow() { return window; }

bool Window::isRunning() { return running; }

void Window::onNotify(Message message) {}

void Window::handleInput() {
	SDL_Event event;
	Message aMessage;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				aMessage.setEvent("MOUSE_LEFT_CLICK");
				send(aMessage);

			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_m:
				GUIManager::Instance()->renderSettingsGUI(true);
			case SDLK_c:
				aMessage.setEvent("CAMERA_STOP");
				send(aMessage);
				break;
			case SDLK_q:
				SDL_Surface * image = SDL_CreateRGBSurface(SDL_SWSURFACE, 800, 600, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);
				glReadBuffer(GL_FRONT);
				glReadPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
				std::string filename = "screenshot_" + std::to_string(Time::Instance()->getRuntime()) + ".bmp";
				SDL_SaveBMP(image, filename.c_str());
				SDL_FreeSurface(image);
				break;
			}
		}
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_A] && state[SDL_SCANCODE_W]) {
		aMessage.setEvent("CAMERA_W_A");
		send(aMessage);
	}
	else if (state[SDL_SCANCODE_D] && state[SDL_SCANCODE_W]) {
		aMessage.setEvent("CAMERA_W_D");
		send(aMessage);
	}

	else if (state[SDL_SCANCODE_D] && state[SDL_SCANCODE_S]) {
		aMessage.setEvent("CAMERA_S_D");
		send(aMessage);
	}

	else if (state[SDL_SCANCODE_A] && state[SDL_SCANCODE_S]) {
		aMessage.setEvent("CAMERA_S_A");
		send(aMessage);
	}

	else if (state[SDL_SCANCODE_LSHIFT]) {
		aMessage.setEvent("CAMERA_LSHIFT");
		send(aMessage);
	}
	else if (state[SDL_SCANCODE_SPACE]) {
		aMessage.setEvent("CAMERA_SPACE");
		send(aMessage);
	}
	else if (state[SDL_SCANCODE_A]) {
		aMessage.setEvent("CAMERA_A");
		send(aMessage);
	}
	else if (state[SDL_SCANCODE_W]) {
		aMessage.setEvent("CAMERA_W");
		send(aMessage);
	}
	else if (state[SDL_SCANCODE_S]) {
		aMessage.setEvent("CAMERA_S");
		send(aMessage);
	}
	else if (state[SDL_SCANCODE_D]) {
		aMessage.setEvent("CAMERA_D");
		send(aMessage);
	}

	else if (state[SDL_SCANCODE_ESCAPE]) {
		running = false;
		Logger::Instance()->write("TERMINATED SUCCESSFULLY");
	}

	else if (state[SDL_SCANCODE_P]) {
		aMessage.setEvent("CAMERA_GET_POSITION");
		send(aMessage);
	}

	else if (state[SDL_SCANCODE_O]) {

	}
}

Window::~Window() {
	SDL_DestroyWindow(window);
	Logger::Instance()->write("TERMINATED SUCCESSFULLY");
}
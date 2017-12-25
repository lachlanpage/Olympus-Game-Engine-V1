//Olympus Game Engine

#include <glad/glad.h>
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "core/Window.h"
#include "utilities\MessageBus.h"

#include "utilities\Camera.h"
#include "utilities\Time.h"

#include "core\Entity.h"

#include "components\CubeGraphicsComponent.h"
#include "components\QuadGraphicsComponent.h"
#include "core\Shader.h"
#include "core\ResourceManager.h"

#include <iostream>

float xoffset = 0;
float yoffset = 0;
void mouse_callback() {
	SDL_Event event;
	if (SDL_PollEvent(&event) == 0) {
		xoffset = event.motion.xrel;
		yoffset = event.motion.yrel;
		if (xoffset < 0.0001 && yoffset < 0.0001) {
			xoffset = 0;
			yoffset = 0;
		}
	}
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_MOUSEMOTION) {
			xoffset = event.motion.xrel;
			yoffset = event.motion.yrel;
		}

		if (event.type == SDL_MOUSEBUTTONDOWN) {
			std::cout << "BUTTON PRESS" << std::endl;
		}
	}
	Camera::Instance()->processMouseMovement(xoffset, -yoffset, true);
}


int main(int argc, char* argv[]) {

	Window *mainWindow = new Window("Olympus Game Engine", 800, 600, MessageBus::Instance());

	Camera::Instance(glm::vec3(6.0f, 15.0f, 24.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), MessageBus::Instance());

	Entity *ent = new Entity(glm::vec3(10,44,400),new CubeGraphicsComponent());
	std::vector<Entity*> entityList;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			entityList.push_back(new Entity(glm::vec3(i, 15, j), new QuadGraphicsComponent()));
		}
	}



	//ResourceManager::Instance()->loadTexture("textures/grass.png");
	//ResourceManager::Instance()->loadTexture("textures/grass.png");

	//std::cout << ResourceManager::Instance()->loadShader("src/shaders/basic.vs", "src/shaders/basic.fs") << std::endl;


	while (mainWindow->isRunning()) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mouse_callback();
		mainWindow->handleInput();

		//Deferred Rendering
		Renderer::Instance()->start();
		for (auto x : entityList)
			x->update();

		Renderer::Instance()->stop();
		//Process Lights 


		//update all messages
		MessageBus::Instance()->notify();
		Time::Instance()->update();
		SDL_GL_SwapWindow(mainWindow->getWindow());
	}

	return 0;
}
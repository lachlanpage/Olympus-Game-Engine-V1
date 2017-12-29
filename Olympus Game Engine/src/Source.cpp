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
#include "components\SphereGraphicsComponent.h"
#include "components\PointLightGraphicsComponent.h"
#include "core\Shader.h"
#include "core\ResourceManager.h"

#include "components\LightComponent.h"

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

	std::vector<Entity*> entityList;
	//test floor and wall 
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j <20; j++) {
			entityList.push_back(new Entity(glm::vec3(i, 0, j), new CubeGraphicsComponent()));

		}
	}
	//test wall
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			entityList.push_back(new Entity(glm::vec3(0, j, i), new CubeGraphicsComponent()));
			entityList.push_back(new Entity(glm::vec3(i, j, 0), new CubeGraphicsComponent()));
		}
	}

	entityList.push_back(new Entity(glm::vec3(5, 1, 5), new CubeGraphicsComponent()));

	entityList.push_back(new Entity(glm::vec3(5, 2, 5), new CubeGraphicsComponent()));

	Entity *quad = new Entity(glm::vec3(0, 0, 0), new QuadGraphicsComponent());

	std::vector<Entity*> lightList;
	Entity *light = new Entity(glm::vec3(2,2,2));
	light->addComponent(new LightComponent(5, glm::vec3(0.0,0.0,1.0)));
	lightList.push_back(light);

	Entity *light2 = new Entity(glm::vec3(14, 4, 9));
	light2->addComponent(new LightComponent(5, glm::vec3(1.0, 0.0, 0.0)));
	lightList.push_back(light2);

	Entity *light4 = new Entity(glm::vec3(19, 7, 12));
	light4->addComponent(new LightComponent(8, glm::vec3(1.0, 1.0, 0.0)));
	lightList.push_back(light4);

	Entity *light5 = new Entity(glm::vec3(0, 3, 12));
	light5->addComponent(new LightComponent(10, glm::vec3(0.4, 0.1, 0.8)));
	lightList.push_back(light5);

	Entity *light6 = new Entity(glm::vec3(13, 5, 18));
	light6->addComponent(new LightComponent(7, glm::vec3(1.0, 0.1, 0.8)));
	lightList.push_back(light6);


	glEnable(GL_DEPTH_TEST);

	double lastTime = SDL_GetTicks();
	int nbFrames = 0;

	while (mainWindow->isRunning()) {
		nbFrames += 1;

		double currentTime = SDL_GetTicks();
		if (currentTime - lastTime > 1000) {
			//a second has passed 
			std::cout << " FPS: " << nbFrames <<  std::endl;
			lastTime = currentTime;
			nbFrames = 0;
		}


		mouse_callback();
		mainWindow->handleInput();

		//Deferred Rendering: start geometry pass 
		Renderer::Instance()->start();
		for (auto x : entityList)
			x->update();
		//debug to draw light spheres bounding
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//newEnt2->update();
		//newEntity->update();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//end geometry pass
		Renderer::Instance()->stop();
		Renderer::Instance()->lightingPassStart();
		//all lights are rendered here
		for (auto light : lightList) {
			light->update();
		}

		Renderer::Instance()->lightingPassStop();
		//draw screen quad with final texture
		quad->update();

		//update all messages
		MessageBus::Instance()->notify();
		Time::Instance()->update();
		SDL_GL_SwapWindow(mainWindow->getWindow());
	}
	return 0;
}
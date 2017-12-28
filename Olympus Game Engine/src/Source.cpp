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

	Entity *ent = new Entity(glm::vec3(10,44,400),new CubeGraphicsComponent());
	std::vector<Entity*> entityList;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			entityList.push_back(new Entity(glm::vec3(i, 0, j), new CubeGraphicsComponent()));
		}
	}

	entityList.push_back(new Entity(glm::vec3(5, 1, 5), new CubeGraphicsComponent()));

	entityList.push_back(new Entity(glm::vec3(5, 2, 5), new CubeGraphicsComponent()));
	entityList.push_back(new Entity(glm::vec3(2, -5, 10), new CubeGraphicsComponent()));
	entityList.push_back(new Entity(glm::vec3(-5, 10, 20), new CubeGraphicsComponent()));
	entityList.push_back(new Entity(glm::vec3(11, 11, 11), new CubeGraphicsComponent()));
	entityList.push_back(new Entity(glm::vec3(0, 10, 50), new CubeGraphicsComponent()));

	Entity *quad = new Entity(glm::vec3(0, 0, 0), new QuadGraphicsComponent());

	Entity *sphere = new Entity(glm::vec3(12,5, 12), new SphereGraphicsComponent());

	Entity *light = new Entity(glm::vec3(12, 5, 12), new PointLightGraphicsComponent());


	Entity *newEntity = new Entity(glm::vec3(2,2,2));
	newEntity->addComponent(new LightComponent(5, glm::vec3(0.0,0.0,1.0)));

	Entity *newEnt2 = new Entity(glm::vec3(9, 10, 9));
	newEnt2->addComponent(new LightComponent(10, glm::vec3(5, 0.0, 0.0)));

	std::vector<Entity*> lightList;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			Entity *entity = new Entity(glm::vec3(i, i + j, j));
			entity->addComponent(new LightComponent(i, glm::vec3(i/ 5, 0.1, 0.3)));
			lightList.push_back(entity);
		}
	}

	//ResourceManager::Instance()->loadTexture("textures/grass.png");
	//ResourceManager::Instance()->loadTexture("textures/grass.png");

	//std::cout << ResourceManager::Instance()->loadShader("src/shaders/basic.vs", "src/shaders/basic.fs") << std::endl;
	glEnable(GL_DEPTH_TEST);

	while (mainWindow->isRunning()) {

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mouse_callback();
		mainWindow->handleInput();

		//Deferred Rendering: start geometry pass 
		Renderer::Instance()->start();
		for (auto x : entityList)
			x->update();
		//sphere->update();
		//light->update();
		//end geometry pass
		Renderer::Instance()->stop();
		Renderer::Instance()->lightingPassStart();
		newEntity->update();
		newEnt2->update();
		Renderer::Instance()->lightingPassStop();
		//glDisable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_ONE, GL_ONE);
		//glEnable(GL_BLEND);
		//glBlendEquation(GL_FUNC_ADD);
		//glBlendFunc(GL_ONE, GL_ONE);
		//glClear(GL_COLOR_BUFFER_BIT);
		//glClear(GL_DEPTH_BUFFER_BIT);
		//newEntity->update();
		//newEnt2->update();
		//light->update();
		//quad->update();
		//light->update();
		quad->update();
		//light->update();
		//Process Lights 


		//update all messages
		MessageBus::Instance()->notify();
		Time::Instance()->update();
		SDL_GL_SwapWindow(mainWindow->getWindow());
	}

	return 0;
}
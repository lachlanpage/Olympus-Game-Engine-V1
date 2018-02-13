//Olympus Game Engine
#include <iostream>
#include <Windows.h>

#include <glad/glad.h>
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utilities/MessageBus.h"
#include "utilities/Camera.h"
#include "utilities/Time.h"

#include "core/Entity.h"
#include "core/Shader.h"
#include "core/ResourceManager.h"
#include "core/Window.h"
#include "utilities/ScriptManager.h"
#include "core/Mouse.h"
#include "core/GUIManager.h"
#include "core/EntityManager.h"

#include "components/CubeGraphicsComponent.h"
#include "components/QuadGraphicsComponent.h"
#include "components/SphereGraphicsComponent.h"
#include "components/PointLightGraphicsComponent.h"
#include "components/PlaneGraphicsComponent.h"
#include "components/LightComponent.h"
#include "components/ModelComponent.h"
#include "components/DirectionalLightComponent.h"

//lua bridge must be after lua import
#include <Lua/lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl_gl3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


int main(int argc, char* argv[]) {

	Window *mainWindow = new Window("Olympus Game Engine", Settings::Instance()->window_width, Settings::Instance()->window_height, MessageBus::Instance());

	Camera::Instance(glm::vec3(6.0f, 15.0f, 24.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), MessageBus::Instance());
	Mouse *raycast = Mouse::Instance(MessageBus::Instance());

	GUIManager::Instance(mainWindow->getWindow());
	EntityManager *entityManager = new EntityManager();
	GUIManager::Instance()->setEntityManager(entityManager);

	//final quad drawn in deferred rendering stage
	Entity *quad = new Entity(glm::vec3(0, 0, 0), new QuadGraphicsComponent());

	//create floor and wall {test scene} 
	//will move to a scene class with manager 
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j <20; j++) {
			Entity *ent = new Entity(glm::vec3(i, 0, j));
			ent->addComponent(new CubeGraphicsComponent());
			entityManager->addEntity(ent);
		}
	}

	for (int i = 0; i < 20; i++) {
		Entity *ent = new Entity(glm::vec3(i, 1,0));
		ent->addComponent(new CubeGraphicsComponent());
		entityManager->addEntity(ent);

		ent = new Entity(glm::vec3(i, 2, 0));
		ent->addComponent(new CubeGraphicsComponent());
		entityManager->addEntity(ent);

		ent = new Entity(glm::vec3(i, 3, 0));
		ent->addComponent(new CubeGraphicsComponent());
		entityManager->addEntity(ent);
	}

	Entity *ent = new Entity(glm::vec3(10, 1, 10));
	ent->addComponent(new CubeGraphicsComponent());
	entityManager->addEntity(ent);

	Entity *ent2 = new Entity(glm::vec3(10, 2, 10));
	ent2->addComponent(new CubeGraphicsComponent());
	entityManager->addEntity(ent2);

	//List of lights in scene
	/*
	std::vector<Entity*> lightList;
	Entity *light = new Entity(glm::vec3(0,2,10));
	light->addComponent(new LightComponent(5, glm::vec3(0.0,0.0,1.0)));

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
	*/

	Entity *sun = new Entity(glm::vec3(10, 10, 10));
	sun->addComponent(new DirectionalLightComponent(glm::vec3(0.7,0.3,0.1)));
	entityManager->addEntity(sun);

	GUIManager::Instance()->renderSceneGraph(true);

	while (mainWindow->isRunning()){
		//comment to disable mouse picking
		//scene graph entity clicking will not work if raycasting is on
		raycast->update(entityManager->getEntityList());
		//light->setPosition(raycast->getCurrentPoint());

		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureKeyboard == 1 || io.WantCaptureMouse == 1) {
			SDL_Event event;
			while (SDL_PollEvent(&event))
				ImGui_ImplSdlGL3_ProcessEvent(&event);
		}
		else {
			Camera::Instance()->processMouseMovement();
			mainWindow->handleInput();
		}

		float currentFrame, timeNow;
		
		currentFrame = SDL_GetTicks();
		//Deferred Rendering: start geometry pass 
		Renderer::Instance()->start();
		entityManager->render();
		Renderer::Instance()->stop();
		//End Geometry Pass
		timeNow = SDL_GetTicks();
		Settings::Instance()->m_geometryPass = timeNow - currentFrame;

		
		currentFrame = SDL_GetTicks();
		//begin Shadow Pass 
		Renderer::Instance()->startShadowMap();
		entityManager->renderShadow();
		Renderer::Instance()->stopShadowMap();
		//End Shadow Pass
		timeNow = SDL_GetTicks();
		Settings::Instance()->m_shadowPass = timeNow - currentFrame;

		//begin lighting pass
		currentFrame = SDL_GetTicks();
		Renderer::Instance()->lightingPassStart();
		entityManager->renderLight();
		Renderer::Instance()->lightingPassStop();
		//End lighting pass
		timeNow = SDL_GetTicks();
		Settings::Instance()->m_lightingPass = timeNow - currentFrame;

		//draw screen quad with final texture
		quad->update();

		//render GUI 
		GUIManager::Instance()->render();

		//update all messages via bus
		MessageBus::Instance()->notify();
		Time::Instance()->update();
		SDL_GL_SwapWindow(mainWindow->getWindow());
	}

	ImGui_ImplSdlGL3_Shutdown();
	return 0;
}
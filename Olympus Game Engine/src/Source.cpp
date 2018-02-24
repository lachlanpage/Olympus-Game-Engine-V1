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
#include "components/ModelComponent.h"
#include "components/ParticleGenerator.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "components/Model.h"
//lua bridge must be after lua import
#include <Lua/lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl_gl3.h>

int main(int argc, char* argv[]) {

	Window *mainWindow = new Window("Olympus Game Engine", Settings::Instance()->window_width, Settings::Instance()->window_height, MessageBus::Instance());

	Camera::Instance(glm::vec3(2.0f, 2.0f,2.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), MessageBus::Instance());
	Mouse *raycast = Mouse::Instance(MessageBus::Instance());

	GUIManager::Instance(mainWindow->getWindow());
	EntityManager *entityManager = new EntityManager();
	GUIManager::Instance()->setEntityManager(entityManager);

	Entity *light6 = new Entity(glm::vec3(3, 2, 3));
	light6->addComponent(new LightComponent(7, glm::vec3(1.0, 0.1, 0.8)));
	entityManager->addEntity(light6);

	//Entity *model2 = new Entity(glm::vec3(0, 0,0));
	//model2->addComponent(new ModelComponent("models/sponza/sponza.obj"));
	//model2->setPosition(glm::vec3(1.0, 1.0, 1.0));
	//model2->setScale(glm::vec3(0.01, 0.01, 0.01));
	//entityManager->addEntity(model2);

	Entity *model = new Entity(glm::vec3(0, 0, 0));
	model->addComponent(new ModelComponent("models/shaderBall/shaderBall.obj"));
	model->setPosition(glm::vec3(1.0, 1.0, 1.0));
	model->setScale(glm::vec3(0.01, 0.01, 0.01));
	entityManager->addEntity(model);

	Entity *sun = new Entity(glm::vec3(10, 10, 10));
	sun->addComponent(new DirectionalLightComponent(glm::vec3(0.7,0.3,0.1)));
	entityManager->addEntity(sun);

	GUIManager::Instance()->renderSceneGraph(true);

	Entity *particle = new Entity(glm::vec3(5, 5,1));
	particle->setScale(glm::vec3(0.2,0.2,0.2));
	particle->addComponent(new ParticleGenerator());
	entityManager->addEntity(particle);

	while (mainWindow->isRunning()){
		//comment to disable mouse picking
		//scene graph entity clicking will not work if raycasting is on
		//raycast->update(entityManager->getEntityList());
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

		Renderer::Instance()->SSAO();
	
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

		Renderer::Instance()->Flush();

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
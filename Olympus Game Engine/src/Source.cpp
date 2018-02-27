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

#include <bullet/btBulletDynamicsCommon.h>

#include "core/PhysicsEngine.h"

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

	Camera::Instance(glm::vec3(-11.0f, 8.0f,0.7f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), MessageBus::Instance());
	Mouse *raycast = Mouse::Instance(MessageBus::Instance());

	GUIManager::Instance(mainWindow->getWindow());
	EntityManager *entityManager = new EntityManager();
	GUIManager::Instance()->setEntityManager(entityManager);

	Entity *light6 = new Entity(glm::vec3(3, 2, 3));
	light6->addComponent(new LightComponent(7, glm::vec3(1.0, 0.1, 0.8)));
	entityManager->addEntity(light6);

	

	//Entity *model = new Entity(glm::vec3(0, 0, 0));
	//model->addComponent(new ModelComponent("models/shaderBall/shaderBall.obj"));
	//model->setPosition(glm::vec3(1.0, 1.0, 1.0));
	//model->setScale(glm::vec3(0.01, 0.01, 0.01));
	//entityManager->addEntity(model);

	Entity *sun = new Entity(glm::vec3(10, 10, 10));
	sun->addComponent(new DirectionalLightComponent(glm::vec3(0.7,0.3,0.1)));
	entityManager->addEntity(sun);



	GUIManager::Instance()->renderSceneGraph(true);

	Entity *model2 = new Entity(glm::vec3(0, 50, 0));
	model2->addComponent(new ModelComponent("models/cube/cube.obj"));
	model2->addComponent(new PhysicsComponent(model2->getPosition(), new btBoxShape(btVector3(0.5, 0.5, 0.5)), 1));
	model2->setScale(glm::vec3(1, 1, 1));
	//model2->setRotation(glm::vec3(100, 100, 100));
	entityManager->addEntity(model2);


	Entity *model3 = new Entity(glm::vec3(1, 0, 0));
	model3->addComponent(new ModelComponent("models/cube/cube.obj"));
	model3->setScale(glm::vec3(25, 0.1, 25));
	entityManager->addEntity(model3);

	Entity *model4 = new Entity(glm::vec3(0.5, 230, 0));
	model4->addComponent(new ModelComponent("models/cube/cube.obj"));
	model4->addComponent(new PhysicsComponent(model4->getPosition(), new btBoxShape(btVector3(0.5, 0.5, 0.5)), 1));
	model4->setScale(glm::vec3(1, 1, 1));
	entityManager->addEntity(model4);


	Entity *acube = new Entity(glm::vec3(1, 300, 0));
	acube->addComponent(new ModelComponent("models/cube/cube.obj"));
	acube->addComponent(new PhysicsComponent(acube->getPosition(), new btBoxShape(btVector3(0.5, 0.5, 0.5)), 1));
	acube->setScale(glm::vec3(1, 1, 1));
	entityManager->addEntity(acube);

	acube = new Entity(glm::vec3(0, 1, 0));
	acube->addComponent(new ModelComponent("models/cube/cube.obj"));
	acube->addComponent(new PhysicsComponent(acube->getPosition(), new btBoxShape(btVector3(0.5, 0.5, 0.5)), 1));
	acube->setScale(glm::vec3(1, 1, 1));
	entityManager->addEntity(acube);


	acube = new Entity(glm::vec3(0, 20, 0));
	acube->addComponent(new ModelComponent("models/cube/cube.obj"));
	acube->addComponent(new PhysicsComponent(acube->getPosition(), new btBoxShape(btVector3(0.5, 0.5, 0.5)), 1));
	acube->setScale(glm::vec3(1, 1, 1));
	entityManager->addEntity(acube);

	acube = new Entity(glm::vec3(0, 22, 0));
	acube->addComponent(new ModelComponent("models/cube/cube.obj"));
	acube->addComponent(new PhysicsComponent(acube->getPosition(), new btBoxShape(btVector3(0.5, 0.5, 0.5)), 1));
	acube->setScale(glm::vec3(1, 1, 1));
	entityManager->addEntity(acube);

	acube = new Entity(glm::vec3(0, 50, 0));
	acube->addComponent(new ModelComponent("models/cube/cube.obj"));
	acube->addComponent(new PhysicsComponent(acube->getPosition(), new btBoxShape(btVector3(0.5, 0.5, 0.5)), 1));
	acube->setScale(glm::vec3(1, 1, 1));
	entityManager->addEntity(acube);







	//add particles last for depth testing stuff
	//Entity *particle = new Entity(glm::vec3(-0.4, 2,-0.2));
	//particle->setScale(glm::vec3(1.2,1.2,1.2));
	//particle->addComponent(new ParticleGenerator());
	//entityManager->addEntity(particle);

	PhysicsEngine *physicsEngine = new PhysicsEngine();
	physicsEngine->addBodies(entityManager->getEntityList());

	while (mainWindow->isRunning()){

		physicsEngine->addBodies(entityManager->getEntityList());
		//comment to disable mouse picking
		//scene graph entity clicking will not work if raycasting is on
		raycast->update(entityManager->getEntityList(), physicsEngine->getDynamicsWorld());
		if (raycast->getLeftClickState()) {
			acube = new Entity(Camera::Instance()->getPosition());
			acube->addComponent(new ModelComponent("models/cube/cube.obj"));
			acube->addComponent(new PhysicsComponent(acube->getPosition(), new btBoxShape(btVector3(0.5, 0.5, 0.5)), 1));
			acube->setScale(glm::vec3(1, 1, 1));
			entityManager->addEntity(acube);
		}
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

		//update physics engine
		physicsEngine->Update();

		//update all messages via bus
		MessageBus::Instance()->notify();
		Time::Instance()->update();
		SDL_GL_SwapWindow(mainWindow->getWindow());
	}

	ImGui_ImplSdlGL3_Shutdown();
	return 0;
}
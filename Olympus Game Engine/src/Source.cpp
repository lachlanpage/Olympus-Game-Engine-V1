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
#include "components\ModelComponent.h"
#include "components\DirectionalLightComponent.h"

#include "utilities\ScriptManager.h"

#include <iostream>
#include <Windows.h>
#include "core/Mouse.h"

#include "core/GUIManager.h"
#include "core/EntityManager.h"
//lua bridge must be after lua
#include <Lua\lua.hpp>
#include <LuaBridge\LuaBridge.h>

//iMGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl_gl3.h>


#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

float xoffset = 0;
float yoffset = 0;




int main(int argc, char* argv[]) {

	ScriptManager::Instance();

	Window *mainWindow = new Window("Olympus Game Engine", Settings::Instance()->window_width, Settings::Instance()->window_height, MessageBus::Instance());

	Camera::Instance(glm::vec3(6.0f, 15.0f, 24.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), MessageBus::Instance());

	Mouse *raycast = Mouse::Instance(MessageBus::Instance());

	//startup GUI 
	GUIManager::Instance(mainWindow->getWindow());
	
	EntityManager *entityManager = new EntityManager();
	
	//test floor and wall 
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

	Entity *quad = new Entity(glm::vec3(0, 0, 0), new QuadGraphicsComponent());

	std::vector<Entity*> lightList;
	Entity *light = new Entity(glm::vec3(0,2,10));
	light->addComponent(new LightComponent(5, glm::vec3(0.0,0.0,1.0)));
	//lightList.push_back(light);

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


	Entity *sun = new Entity(glm::vec3(10, 10, 10));
	sun->addComponent(new DirectionalLightComponent(glm::vec3(0.7,0.3,0.1)));

	while (mainWindow->isRunning()) {
		//mouse picking
		raycast->update(entityManager->getEntityList());
		light->setPosition(raycast->getCurrentPoint());

		if(raycast->blockClickID == -1) {
			GUIManager::Instance()->renderEntityEditor(false);
		}
		else {
			GUIManager::Instance()->renderEntityEditor(true);
		}

		//check block selections will move to entity manager class ? 
		int blockID = raycast->blockClickID;
		for (auto entity : entityManager->getEntityList()) {
			if (entity->m_ID == blockID) {
				entity->is_selected = true;
			}
			else {
				entity->is_selected = false;
			}
		}
		
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



		//create gui 
		//create gui 
		/*
		ImGui_ImplSdlGL3_NewFrame(mainWindow->getWindow());
		//Creation of imgui entity frame
		int ID;
		glm::vec3 blockPos;
		glm::vec3 blockScale;
		glm::vec3 blockRotation;
		Entity *entityAdd = nullptr;
		if (raycast->blockClickID != -1) {
			for (auto entity : entityList) {
				if (raycast->blockClickID == entity->m_ID) {
					ID = entity->m_ID;
					blockPos = entity->getPosition();
					blockScale = entity->getScale();
					blockRotation = entity->getRotation();
					entityAdd = entity;
				}
			}
		}
		else {
			ID = 0;
			blockPos = glm::vec3(0, 0, 0);
			blockScale = glm::vec3(0, 0, 0);
			blockRotation = glm::vec3(0, 0, 0);
		}

		
		static float value = 10;
		static char buffer[50] = {};
		std::vector<char> buffman;
		static float buf1[64] = { 0 };

		ImGui::Begin("Entity Inspector");
		ImGui::Spacing();
		ImGui::PushItemWidth(100);
		ImGui::Text("Entity");  ImGui::SameLine(); ImGui::Text(std::to_string(ID).c_str());
		ImGui::PopItemWidth();
		if (ImGui::CollapsingHeader("Transform")) {	
			ImGui::Text("Position");
			ImGui::NewLine();
			ImGui::PushItemWidth(100);
			ImGui::Text("X"); ImGui::SameLine(); ImGui::PushID(0); if (ImGui::InputFloat("", &blockPos.x, 0, 0, 3)) { ImGui::SetKeyboardFocusHere(-1); }  ImGui::PopID();ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine(); ImGui::PushID(1);if(ImGui::InputFloat("", &blockPos.y, 0,0,3)) { ImGui::SetKeyboardFocusHere(-1); } ImGui::PopID();ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine(); ImGui::PushID(2); if(ImGui::InputFloat("", &blockPos.z, 0,0,3)) { ImGui::SetKeyboardFocusHere(-1); } ImGui::PopID();
			ImGui::PopItemWidth();
			ImGui::NewLine();
			ImGui::Text("Scale");
			ImGui::NewLine();
			ImGui::PushItemWidth(100);
			ImGui::Text("X"); ImGui::SameLine();  ImGui::PushID(3); if (ImGui::InputFloat("", &blockScale.x, 0, 0, 3)) { ImGui::SetKeyboardFocusHere(-1); }   ImGui::PopID();ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine();  ImGui::PushID(4);if (ImGui::InputFloat("", &blockScale.y, 0, 0, 3)) { ImGui::SetKeyboardFocusHere(-1); } ImGui::PopID(); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine();  ImGui::PushID(5);if (ImGui::InputFloat("", &blockScale.z, 0, 0, 3)) { ImGui::SetKeyboardFocusHere(-1); } ImGui::PopID();
			ImGui::PopItemWidth();
			ImGui::NewLine();
			ImGui::Text("Rotation");
			ImGui::NewLine();
			ImGui::PushItemWidth(100);
			ImGui::Text("X"); ImGui::SameLine(); ImGui::PushID(6); if (ImGui::InputFloat("", &blockRotation.x, 0, 0, 3)) { ImGui::SetKeyboardFocusHere(-1); }   ImGui::PopID();ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine();  ImGui::PushID(7);if (ImGui::InputFloat("", &blockRotation.y, 0, 0, 3)) { ImGui::SetKeyboardFocusHere(-1); } ImGui::PopID(); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine();  ImGui::PushID(8);if (ImGui::InputFloat("", &blockRotation.z, 0, 0, 3)) { ImGui::SetKeyboardFocusHere(-1); } ImGui::PopID();
			ImGui::PopItemWidth();
			ImGui::NewLine();
			//ImGui::SliderFloat("label", &value, 0, 100);

			//if gui updates values we gotta set them here 
			if(entityAdd != nullptr){
				entityAdd->setPosition(blockPos);
				entityAdd->setScale(blockScale);
				entityAdd->setRotation(blockRotation);
			}

		}

		if (ImGui::CollapsingHeader("Textures")) {
			static char buf[100] = "";
			ImGui::Text("Albedo");
			ImGui::PushItemWidth(100);
			ImGui::Image((void*)albedo_texture, ImVec2(100, 100)); ImGui::SameLine();
			ImGui::InputText("filename", buf, 100);
			ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), buf);
			ImGui::PopItemWidth();
			
		}
		ImGui::End();

		*/
		float currentFrame = SDL_GetTicks();
		//Deferred Rendering: start geometry pass 
		Renderer::Instance()->start();
		entityManager->render();
		//for (auto x : entityList)
		//	x->update();
		//end geometry pass
		Renderer::Instance()->stop();
		float timeNow = SDL_GetTicks();
		Settings::Instance()->m_geometryPass = timeNow - currentFrame;
		
		currentFrame = SDL_GetTicks();
		//Shadow Pass 

		Renderer::Instance()->startShadowMap();
		entityManager->renderShadow();
		Renderer::Instance()->stopShadowMap();
		timeNow = SDL_GetTicks();
		Settings::Instance()->m_shadowPass = timeNow - currentFrame;

		//lighting pass

		currentFrame = SDL_GetTicks();
		Renderer::Instance()->lightingPassStart();
		
		light2->update();
		sun->update();
		Renderer::Instance()->lightingPassStop();
		timeNow = SDL_GetTicks();
		Settings::Instance()->m_lightingPass = timeNow - currentFrame;

		//draw screen quad with final texture
		quad->update();

		//render crosshair

		//render GUI 
		GUIManager::Instance()->render();

		//update all messages
		MessageBus::Instance()->notify();
		Time::Instance()->update();
		SDL_GL_SwapWindow(mainWindow->getWindow());

	}
	ImGui_ImplSdlGL3_Shutdown();
	//lua_close(L);
	return 0;
}
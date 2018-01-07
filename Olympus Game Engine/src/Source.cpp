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

#include <iostream>

#include "core/Mouse.h"

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

void ImGui_ImplSdl_CharCallback(unsigned int c) {

}


int main(int argc, char* argv[]) {



	/*
	//Test lua
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	if (luaL_dofile(L, "src/scripts/test.lua")) {
		const char* err = lua_tostring(L, -1);
		std::cout << err << std::endl;
	}
	lua_pcall(L, 0, 0, 0);
	luabridge::LuaRef s = luabridge::getGlobal(L, "something");
	std::string luaString = s.cast<std::string>();
	std::cout << luaString << std::endl;

	//lua_state *L;
	//lua_State *L = luaL_newstate();
	//luaL_openlibs(L);

	*/



	Window *mainWindow = new Window("Olympus Game Engine", Settings::Instance()->window_width, Settings::Instance()->window_height, MessageBus::Instance());




	Camera::Instance(glm::vec3(6.0f, 15.0f, 24.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), MessageBus::Instance());

	Mouse *raycast = Mouse::Instance(MessageBus::Instance());

	std::vector<Entity*> entityList;
	//test floor and wall 
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j <20; j++) {
			Entity *ent = new Entity(glm::vec3(i, 0, j));
			ent->addComponent(new CubeGraphicsComponent());
		
			entityList.push_back(ent);
		}
	}

	Entity *ent = new Entity(glm::vec3(10, 1, 10));
	ent->addComponent(new CubeGraphicsComponent());
	entityList.push_back(ent);

	Entity *ent2 = new Entity(glm::vec3(10, 2, 10));
	ent2->addComponent(new CubeGraphicsComponent());
	entityList.push_back(ent2);

	Entity *ent3 = new Entity(glm::vec3(10, 3, 10));
	ent3->addComponent(new CubeGraphicsComponent());
	entityList.push_back(ent3);

	ent3 = new Entity(glm::vec3(11, 1, 8));
	ent3->addComponent(new CubeGraphicsComponent());
	entityList.push_back(ent3);

	ent3 = new Entity(glm::vec3(4, 2, 1));
	ent3->addComponent(new CubeGraphicsComponent());
	entityList.push_back(ent3);

	ent3 = new Entity(glm::vec3(4, 1, 1));
	ent3->addComponent(new CubeGraphicsComponent());
	entityList.push_back(ent3);




	/*
	//test wall
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			entityList.push_back(new Entity(glm::vec3(0, j, i), new CubeGraphicsComponent()));
			entityList.push_back(new Entity(glm::vec3(i, j, 0), new CubeGraphicsComponent()));
		}
	}

	entityList.push_back(new Entity(glm::vec3(5, 1, 5), new CubeGraphicsComponent()));

	entityList.push_back(new Entity(glm::vec3(5, 2, 5), new CubeGraphicsComponent()));
	*/
	Entity *quad = new Entity(glm::vec3(0, 0, 0), new QuadGraphicsComponent());
	Entity *sphere = new Entity(glm::vec3(5, 5, 5), new SphereGraphicsComponent());

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


	double lastTime = SDL_GetTicks();
	int nbFrames = 0;

	//Imgui styling 
	ImGui::StyleColorsDark();

	bool renderIMGUI = true;

	ImGui_ImplSdlGL3_Init(mainWindow->getWindow());

	while (mainWindow->isRunning()) {

		//ImGuiIO& io = ImGui::GetIO();
		//io.WantCaptureKeyboard = false;

		//testing of creating gui 

		nbFrames += 1;

		//mouse picking
		raycast->update(entityList);
		//std::cout << raycast->getCurrentPoint().x << " " << raycast->getCurrentPoint().y << std::endl;
		light->setPosition(raycast->getCurrentPoint());


		if (raycast->blockClickID == -1) {
			renderIMGUI = false;	
		}
		else {
			renderIMGUI = true;
		}

		//check block selections will move to entity manager class ? 
		int blockID = raycast->blockClickID;
		for (auto entity : entityList) {
			if (entity->m_ID == blockID) {
				entity->is_selected = true;
			}
			else {
				entity->is_selected = false;
			}
		}
		
		//std::cout << light->getPosition().x << " " << light->getPosition().y << " " << light->getPosition().z << std::endl;
		double currentTime = SDL_GetTicks();
		if (currentTime - lastTime > 1000) {
			//a second has passed 
			std::cout << " FPS: " << nbFrames <<  std::endl;
			lastTime = currentTime;
			nbFrames = 0;
		}

		/*
		if (renderIMGUI == true && ImGui::IsMouseHoveringAnyWindow() == true) {
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				ImGui_ImplSdlGL3_ProcessEvent(&event);
			}
			//ImGuiIO& io = ImGui::GetIO();
			//io.WantCaptureKeyboard = true;
			//io.WantCaptureMouse = true;
			
			//std::cout << io.WantCaptureKeyboard << std::endl;

		

		}
		else {
			//std::cout << "THIS SHOULD NOT BE CALLED" << std::endl;
			ImGuiIO& io = ImGui::GetIO();
			std::cout << io.WantCaptureKeyboard << std::endl;
			Camera::Instance()->processMouseMovement();
			mainWindow->handleInput();
		}

		*/

		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureKeyboard == 1 || io.WantCaptureMouse == 1) {
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				ImGui_ImplSdlGL3_ProcessEvent(&event);
			}

		}
		
		else {
			Camera::Instance()->processMouseMovement();
			mainWindow->handleInput();
		}



		//create gui 
		//create gui 

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
		ImGui::End();



		//if (io.WantCaptureKeyboard == 1) {

		//}

		//Deferred Rendering: start geometry pass 
		Renderer::Instance()->start();
		for (auto x : entityList)
			x->update();

		//end geometry pass
		Renderer::Instance()->stop();

		//Shadow Pass 
		Renderer::Instance()->startShadowMap();
		for (auto x : entityList)
			x->updateShadow();
		Renderer::Instance()->stopShadowMap();

		//lighting pass
		Renderer::Instance()->lightingPassStart();
		light->update();
		sun->update();

		Renderer::Instance()->lightingPassStop();
		//draw screen quad with final texture
		quad->update();

		//render crosshair

		//render GUI 
		if (renderIMGUI)
			ImGui::Render();
		else
			ImGui::EndFrame();
		//update all messages
		MessageBus::Instance()->notify();
		Time::Instance()->update();
		SDL_GL_SwapWindow(mainWindow->getWindow());
	}
	ImGui_ImplSdlGL3_Shutdown();
	//lua_close(L);
	return 0;
}
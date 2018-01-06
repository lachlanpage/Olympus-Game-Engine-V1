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




int main(int argc, char* argv[]) {



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





	Window *mainWindow = new Window("Olympus Game Engine", 800, 600, MessageBus::Instance());




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

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 5.3f;
	style.FrameRounding = 2.3f;
	style.ScrollbarRounding = 0;

	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
	//style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	//style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
	//style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	while (mainWindow->isRunning()) {

		ImGuiIO& io = ImGui::GetIO();
		io.WantCaptureKeyboard = false;

		//testing of creating gui 
		//create gui 
		ImGui_ImplSdlGL3_Init(mainWindow->getWindow());

		ImGui_ImplSdlGL3_NewFrame(mainWindow->getWindow());

		ImGui::Begin("Entity Inspector");
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("Transform")) {
			static float buf1[64] = { 0 };
			ImGui::Text("Position");
			ImGui::NewLine();
			ImGui::PushItemWidth(100);
			ImGui::Text("X"); ImGui::SameLine();  ImGui::InputFloat("", buf1, 0,0,3);  ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("", buf1, 0,0,3); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("", buf1, 0,0,3);
			ImGui::PopItemWidth();
			ImGui::NewLine();
			ImGui::Text("Rotation");
			ImGui::NewLine();
			ImGui::PushItemWidth(100);
			ImGui::Text("X"); ImGui::SameLine();  ImGui::InputFloat("", buf1, 0, 0, 3);  ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("", buf1, 0, 0, 3); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("", buf1, 0, 0, 3);
			ImGui::PopItemWidth();
			ImGui::NewLine();
			ImGui::Text("Scale");
			ImGui::NewLine();
			ImGui::PushItemWidth(100);
			ImGui::Text("X"); ImGui::SameLine();  ImGui::InputFloat("", buf1, 0, 0, 3);  ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("", buf1, 0, 0, 3); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("", buf1, 0, 0, 3);
			ImGui::PopItemWidth();
			ImGui::NewLine();
		}
		ImGui::End();


		//io.WantCaptureKeyboard = true;
		//io.WantCaptureMouse = true;

		nbFrames += 1;

		//mouse picking
		raycast->update(entityList);
		//std::cout << raycast->getCurrentPoint().x << " " << raycast->getCurrentPoint().y << std::endl;
		light->setPosition(raycast->getCurrentPoint());

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

		Camera::Instance()->processMouseMovement();
		mainWindow->handleInput();

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

		ImGui::Render();
	
		//update all messages
		MessageBus::Instance()->notify();
		Time::Instance()->update();
		SDL_GL_SwapWindow(mainWindow->getWindow());
	}
	lua_close(L);
	return 0;
}
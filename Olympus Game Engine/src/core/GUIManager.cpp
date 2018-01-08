#include "GUIManager.h"

GUIManager* GUIManager::m_Instance = 0;

GUIManager* GUIManager::Instance() {
	if (m_Instance == 0)
		Logger::Instance()->write("GUI INCORRECT INSTANCE CALL");
	return m_Instance;
}

GUIManager* GUIManager::Instance(SDL_Window* window) {
	if (m_Instance == 0)
		m_Instance = new GUIManager(window);
	return m_Instance;
}

GUIManager::GUIManager(SDL_Window* window) {
	m_window = window;
	ImGui_ImplSdlGL3_Init(m_window);
	ImGui::StyleColorsDark();

	//setting defautls 
	m_renderSettingsEditor = false;
	m_renderEntityEditor = false;
}

void GUIManager::renderEntityEditor(bool flag) {
	m_renderEntityEditor = flag;
}
void GUIManager::renderSettingsGUI(bool flag) {
	m_renderSettingsEditor = flag;
}
void GUIManager::generateEntityEditor() {
	//here goes all the code for entity editor 
	static char buf[50] = "";

	ImGui::Begin("BLAHBlAH");
	ImGui::Text("THIS IS A WINDOW 2");
	ImGui::InputText("abcd", buf, 50);


	//Creation of imgui entity frame
	/*
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
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::PushID(1);if (ImGui::InputFloat("", &blockPos.y, 0, 0, 3)) { ImGui::SetKeyboardFocusHere(-1); } ImGui::PopID();ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::PushID(2); if (ImGui::InputFloat("", &blockPos.z, 0, 0, 3)) { ImGui::SetKeyboardFocusHere(-1); } ImGui::PopID();
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
		if (entityAdd != nullptr) {
			entityAdd->setPosition(blockPos);
			entityAdd->setScale(blockScale);
			entityAdd->setRotation(blockRotation);
		}

	}

	//if (ImGui::CollapsingHeader("Textures")) {
	//	static char buf[100] = "";
	//	ImGui::Text("Albedo");
	//	ImGui::PushItemWidth(100);
	//	ImGui::Image((void*)albedo_texture, ImVec2(100, 100)); ImGui::SameLine();
	//	ImGui::InputText("filename", buf, 100);
	//	ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), buf);
	//	ImGui::PopItemWidth();
	//}

	*/

	ImGui::End();
}


void GUIManager::generateSettingsGUI() {
	ImGui::Begin("Settings", &m_renderSettingsEditor);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Geometry Pass %.3f ms/frame (%.1f FPS)", Settings::Instance()->m_geometryPass, 1/(Settings::Instance()->m_geometryPass/1000));
	ImGui::Text("Shadow Pass %.3f ms/frame (%.1f FPS)", Settings::Instance()->m_shadowPass, 1 / (Settings::Instance()->m_shadowPass / 1000));
	ImGui::Text("Lighting Pass %.3f ms/frame (%.1f FPS)", Settings::Instance()->m_lightingPass, 1 / (Settings::Instance()->m_lightingPass / 1000));

	if (ImGui::CollapsingHeader("Display Mode")) {
		ImGui::RadioButton("Albedo", &Settings::Instance()->m_textureSelector, 0);
		ImGui::RadioButton("Normal", &Settings::Instance()->m_textureSelector,1);
		ImGui::RadioButton("Position", &Settings::Instance()->m_textureSelector, 2);
		ImGui::RadioButton("Shadow Map", &Settings::Instance()->m_textureSelector, 6);
		ImGui::RadioButton("Specular", &Settings::Instance()->m_textureSelector, 7);
		ImGui::RadioButton("Combined", &Settings::Instance()->m_textureSelector, 5);
	}
	ImGui::End();
}

void GUIManager::render() {
	ImGui_ImplSdlGL3_NewFrame(m_window);

	if (m_renderEntityEditor) {
		generateEntityEditor();
	}

	if (m_renderSettingsEditor) {
		generateSettingsGUI();
	}

	ImGui::Render();
}
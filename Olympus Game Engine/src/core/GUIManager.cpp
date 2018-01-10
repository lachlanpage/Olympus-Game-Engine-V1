#include "GUIManager.h"
#include "../components/CubeGraphicsComponent.h"

#include <Windows.h>
#include <Commdlg.h>

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

void GUIManager::setEntityEditor(Entity* entity) {
	m_entity = entity;
}

void GUIManager::setEntityManager(EntityManager* entityManager) {
	m_entitymanager = entityManager;
}
void GUIManager::generateEntityEditor() {
	//here goes all the code for entity editor 
	int ID = m_entity->m_ID;
	glm::vec3 blockPos = m_entity->getPosition();
	glm::vec3 blockScale = m_entity->getScale();
	glm::vec3 blockRotation = m_entity->getRotation();

	if (m_entity == nullptr) {
		ID = 0;
		blockPos = glm::vec3(0, 0, 0);
		blockScale = glm::vec3(0, 0, 0);
		blockRotation = glm::vec3(0, 0, 0);
	}

	ImGui::Begin("Entity Inspector");
	ImGui::Spacing();

	/*
	if (ImGui::CollapsingHeader("Create Entity")) {
		glm::vec3 newEntityBlock;
		ImGui::PushItemWidth(100);
		ImGui::Text("X"); ImGui::SameLine(); ImGui::PushID(17); if (ImGui::InputFloat("", &newEntityBlock.x, 0, 0, 3)) { ImGui::SetKeyboardFocusHere(-1); }  ImGui::PopID();ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::PushID(18);if (ImGui::InputFloat("", &newEntityBlock.y, 0, 0, 3)) { ImGui::SetKeyboardFocusHere(-1); } ImGui::PopID();ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::PushID(19); if (ImGui::InputFloat("", &newEntityBlock.z, 0, 0, 3)) { ImGui::SetKeyboardFocusHere(-1); } ImGui::PopID();
		ImGui::PopItemWidth();

		if (ImGui::Button("Create Entity")) {
			m_entitymanager->addEntity(new Entity(newEntityBlock));
		}
	}

	*/

	CubeGraphicsComponent *entityGraphics = m_entity->GetComponent  <CubeGraphicsComponent> ();

	ImGui::PushItemWidth(100);
	ImGui::Text("Entity");  ImGui::SameLine(); ImGui::Text(std::to_string(ID).c_str());
	ImGui::PopItemWidth();

	if (ImGui::CollapsingHeader("Textures")) {
		ImGui::Text("Albedo");
		ImGui::PushID(100);
		if (ImGui::ImageButton((void*)entityGraphics->getAlbedoTexture(), ImVec2(100, 100))) {
			TCHAR*DefaultExtension = 0;
			TCHAR*FileName = new TCHAR[MAX_PATH];
			TCHAR*Filter = 0;
			int FilterIndex = 0;
			int Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			TCHAR*InitialDir = 0;
			HWND Owner = 0;
			TCHAR*Title = 0;

			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = Owner;

			ofn.lpstrDefExt = DefaultExtension;
			ofn.lpstrFile = FileName;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = Filter;
			ofn.nFilterIndex = FilterIndex;
			ofn.lpstrInitialDir = InitialDir;
			ofn.lpstrTitle = Title;
			ofn.Flags = Flags;

			GetOpenFileName(&ofn);
			//need to fix this hacky way and figure out how to use relative paths
			std::string filename = FileName;
			if (filename.size() != 0) {
				std::string textureFile = filename.substr(77);
				textureFile[8] = '/';
				entityGraphics->setAlbedoTexture(textureFile);
			}
		}
		ImGui::PopID();
		ImGui::TextColored(ImVec4(1, 0, 0, 1), entityGraphics->getAlbedoTextureFilename().c_str());
		
		ImGui::NewLine();

		ImGui::Text("Specular");
		ImGui::PushID(101);
		if (ImGui::ImageButton((void*)entityGraphics->getSpecularTexture(), ImVec2(100, 100))) {
			TCHAR*DefaultExtension = 0;
			TCHAR*FileName = new TCHAR[MAX_PATH];
			TCHAR*Filter = 0;
			int FilterIndex = 0;
			int Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			TCHAR*InitialDir = 0;
			HWND Owner = 0;
			TCHAR*Title = 0;

			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = Owner;

			ofn.lpstrDefExt = DefaultExtension;
			ofn.lpstrFile = FileName;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = Filter;
			ofn.nFilterIndex = FilterIndex;
			ofn.lpstrInitialDir = InitialDir;
			ofn.lpstrTitle = Title;
			ofn.Flags = Flags;

			GetOpenFileName(&ofn);
			//need to fix this hacky way and figure out how to use relative paths
			std::string filename = FileName;
			if (filename.size() != 0) {
				std::string textureFile = filename.substr(77);
				textureFile[8] = '/';

				entityGraphics->setSpecularTexture(textureFile);
			}
		}
		ImGui::PopID();
		ImGui::TextColored(ImVec4(1, 0, 0, 1), entityGraphics->getSpecularTextureFilename().c_str());

		ImGui::NewLine();


	}
	if (ImGui::CollapsingHeader("Transform")) {
		ImGui::Text("Position");
		ImGui::NewLine();
		ImGui::PushItemWidth(100);
		ImGui::Text("X"); ImGui::SameLine(); ImGui::PushID(0); if (ImGui::InputFloat("", &blockPos.x, 0, 0, 3)) { ImGui::SetKeyboardFocusHere(-1); }  ImGui::PopID();ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::PushID(1);if (ImGui::InputFloat("", &blockPos.y, 0, 0, 3)) { ImGui::SetKeyboardFocusHere(-1); } ImGui::PopID();ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::PushID(2); if (ImGui::InputFloat("", &blockPos.z, 0, 0, 3)) { ImGui::SetKeyboardFocusHere(-1); } ImGui::PopID();
		ImGui::NewLine();
		ImGui::PushID(12); ImGui::SliderFloat("", &blockPos.x, -10, 10); ImGui::PopID(); ImGui::SameLine(); ImGui::PushID(13); ImGui::VSliderFloat("asd", ImVec2(100, 100), &blockPos.y, -10, 10); ImGui::PopID(); ImGui::SameLine();
		ImGui::PushID(14); ImGui::SliderFloat("", &blockPos.z, -10, 10); ImGui::PopID();

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
		ImGui::PushItemWidth(100);
		ImGui::Text("X"); ImGui::SameLine(); ImGui::PushID(9); ImGui::SliderFloat("X", &blockRotation.x, 0, 360); ImGui::PopID();
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::PushID(10); ImGui::SliderFloat("Y", &blockRotation.y, 0, 360); ImGui::PopID();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::PushID(11); ImGui::SliderFloat("Z", &blockRotation.z, 0, 360);ImGui::PopID();
		ImGui::PopItemWidth();

		//if gui updates values we gotta set them here 
		if (m_entity!= nullptr) {
			m_entity->setPosition(blockPos);
			m_entity->setScale(blockScale);
			m_entity->setRotation(blockRotation);
		}
	}

	ImGui::End();
}


void GUIManager::renderSceneGraph(bool flag) {
	m_renderSceneGraph = flag;
}

void GUIManager::generateSceneGraph() {
	ImGui::Begin("Scene Graph");
	ImGui::Text("Scene Graph");
	for (auto entity : m_entitymanager->getEntityList()) {
		std::string name = "Entity: " + std::to_string((entity->m_ID));
		if (ImGui::TreeNode(name.c_str())) {
			ImGui::Text(std::to_string(entity->getPosition().x).c_str());
			ImGui::TreePop();
		}
	}
	if (ImGui::TreeNode("abcd")) {
		ImGui::Button("ASSDS");
		ImGui::TreePop();
	}
	ImGui::End();
}

void GUIManager::generateSettingsGUI() {
	ImGui::Begin("Settings", &m_renderSettingsEditor);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Geometry Pass %.3f ms/frame (%.1f FPS)", Settings::Instance()->m_geometryPass, 1/(Settings::Instance()->m_geometryPass/1000));
	ImGui::Text("Shadow Pass %.3f ms/frame (%.1f FPS)", Settings::Instance()->m_shadowPass, 1 / (Settings::Instance()->m_shadowPass / 1000));
	ImGui::Text("Lighting Pass %.3f ms/frame (%.1f FPS)", Settings::Instance()->m_lightingPass, 1 / (Settings::Instance()->m_lightingPass / 1000));
	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::Text("Entity Count: "); ImGui::SameLine(); ImGui::Text(std::to_string(m_entitymanager->getEntityList().size()).c_str());
	ImGui::NewLine();
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

	if (m_renderSceneGraph) {
		generateSceneGraph();
	}

	ImGui::Render();
}
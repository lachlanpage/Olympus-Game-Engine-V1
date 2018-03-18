#ifndef GUIMANAGER_H
#define GUIMANAGER_H
#include <SDL\SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl_gl3.h>

#include "../utilities/Logger.h"

#include <iostream>
#include "../utilities/Settings.h"
#include "Entity.h"
#include "EntityManager.h"

#include "../components/CubeGraphicsComponent.h"
#include "../components/DirectionalLightComponent.h"
#include "../components/LightComponent.h"
#include "../components/ParticleGenerator.h"

#include "../components/Mesh.h"

class GUIManager {
	//Singleton class to hold all global consts across engine {width, height of window etc}
public:
	static GUIManager* Instance();
	static GUIManager* Instance(SDL_Window* window);

	//setters for render window flags 
	void renderEntityEditor(bool);

	void setEntityEditor(Entity*);
	void setEntityManager(EntityManager*);
	void setDirectionalLightEditor(DirectionalLightComponent*);
	void setPointLightEditor(LightComponent*);
	void setCubeGraphics(CubeGraphicsComponent*);

	void setParticleEditor(ParticleGenerator*);
	void renderParticleEditor(bool);


	void renderSettingsGUI(bool);
	void renderSceneGraph(bool);
	void renderDirectionalLightEditor(bool);
	void renderPointLightEditor(bool);
	void renderCubeGraphics(bool);

	void render();
private:
	//generate the gui frames
	void generateEntityEditor();
	void generateParticleEditor();
	void generateSettingsGUI();
	void generateSceneGraph();
	void generateDirectionalLightGUI();
	void generatePointLightGUI();
	void generateCubeGraphicsEditor();


	SDL_Window* m_window;
	bool m_renderEntityEditor;
	bool m_renderSettingsEditor;
	bool m_renderSceneGraph;
	bool m_renderDirectionalLight;
	bool m_renderPointLight;
	bool m_renderParticleEditor;
	bool m_renderCubeGraphicsEditor;

	//keeping track of what scene graph entities to display 
	bool scenegraph_showLights;
	bool scenegraph_showModels;

	EntityManager *m_entitymanager;
	Entity* m_entity;
	DirectionalLightComponent* m_directionalLight;
	LightComponent* m_pointLight;
	ParticleGenerator* m_particleEditor;
	CubeGraphicsComponent* cubeGraphics;

	GUIManager(SDL_Window*);
	static GUIManager* m_Instance;
};
#endif




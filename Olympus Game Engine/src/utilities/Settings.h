#ifndef SETTINGS_H 
#define SETTINGS_H 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Settings {
	//Singleton class to hold all global consts across engine {width, height of window etc}
public:
	static Settings* Instance();
	glm::mat4 projection;
	int window_height;
	int window_width;
	glm::vec3 lightDir;

	void setLightDirection(glm::vec3);

	glm::mat4 projectionMatrix;
	glm::mat4 depthViewMatrix;
	glm::mat4 depthModelMatrix;
	glm::mat4 depthMVP;
	glm::mat4 biasMatrix;
	glm::mat4 lightSpaceMatrix;
private:
	Settings();
	Settings(Settings const&) {};
	Settings& operator=(Settings const&) {};
	static Settings* m_Instance;
};
#endif




#ifndef SETTINGS_H 
#define SETTINGS_H 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Settings {
public:
	static Settings* Instance();
	glm::mat4 projection;
private:
	Settings();
	Settings(Settings const&) {};
	Settings& operator=(Settings const&) {};
	static Settings* m_Instance;

};

#endif




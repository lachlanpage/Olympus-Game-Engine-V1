#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>
#include "../utilities/stb_image.h"
#include "../utilities/Logger.h"
#include "../core/Shader.h"

class ResourceManager {
public:
	static ResourceManager* m_Instance;
	static ResourceManager* Instance();

	unsigned int loadTexture(std::string);
	Shader* loadShader(std::string, std::string);

private:
	ResourceManager();
	unsigned int loadImage(std::string filename);

	std::map<std::string, unsigned int> m_texturesLoaded;
	std::map<std::string, unsigned int>::iterator m_texturesIterator;

	std::map<std::pair<std::string, std::string>, Shader*> m_shadersLoaded;
	std::map<std::pair<std::string, std::string>, Shader*>::iterator m_shadersIterator;

};

#endif
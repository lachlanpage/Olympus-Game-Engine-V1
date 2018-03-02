#include "ResourceManager.h"

ResourceManager* ResourceManager::m_Instance = nullptr;

ResourceManager* ResourceManager::Instance() {
	if (m_Instance == nullptr)
		m_Instance = new ResourceManager();
	return m_Instance;
}

ResourceManager::ResourceManager() {}


void ResourceManager::printTextures() {

	m_texturesIterator = m_texturesLoaded.begin();
	while (m_texturesIterator != m_texturesLoaded.end()) {
		std::cout << m_texturesIterator->first << " " << m_texturesIterator->second << std::endl;
		std::cout << m_texturesIterator->first << " " << &m_texturesIterator->second << std::endl;
		m_texturesIterator++;
	}
}

unsigned int ResourceManager::loadTexture(std::string filename) {
	//check if texture has already been loaded, if it has not - load image and then store in loaded texture otherwise return image
	m_texturesIterator = m_texturesLoaded.find(filename);
	if (m_texturesIterator != m_texturesLoaded.end()) {
		//texture already exists
		Logger::Instance()->write("Texture: " + filename + " already exists");
		return m_texturesIterator->second;
	}
	else {
		//texture not found, create texture and insert it into textures loaded
		unsigned int texture = loadImage(filename);
		m_texturesLoaded.insert(std::pair<std::string, unsigned int>(filename, texture));
		return texture;
	}
}

unsigned int ResourceManager::loadImage(std::string filename) {
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;

	unsigned int texture;
	glGenTextures(1, &texture);

	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

	if (!data) {
		Logger::Instance()->write(stbi_failure_reason() + filename);
		Logger::Instance()->write("Failed to load Image: " + filename);
		std::cout << "Image failed to load: " << filename<< std::endl;
		stbi_image_free(data);
	}
	else {
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0,format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Logger::Instance()->write("LOADED IMAGE: " + filename);
		std::cout << "LOADED IMAGE: " << filename << std::endl;
		stbi_image_free(data);
		//return texture;
	}
	return texture;
}

Shader* ResourceManager::loadShader(std::string vert, std::string frag) {
	m_shadersIterator = m_shadersLoaded.find(std::pair<std::string, std::string>(vert, frag));
	if (m_shadersIterator != m_shadersLoaded.end()) {
		//Shader already exists
		Logger::Instance()->write("Shader EXISTS: vert - " + vert + " frag - " + frag);
		return m_shadersIterator->second;
	}
	else {
		//shader not found, create shader and insert into shaders
		Shader* shader = new Shader(vert.c_str(), frag.c_str());
		m_shadersLoaded.insert(std::make_pair(std::make_pair(vert, frag), shader));
		Logger::Instance()->write("Shader CREATED: vert - " + vert + " frag - " + frag);
		return shader;
	}
}
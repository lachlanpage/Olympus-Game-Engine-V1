#ifndef RENDERER_H
#define RENDERER_H
#include <glad/glad.h>
#include <iostream>
class Renderer {
public:
	static Renderer* m_Instance;
	static Renderer* Instance();
	void start();
	void stop();
	void render(GLenum, GLint, GLsizei);
	
private:
	unsigned int m_fbo;
	Renderer();

	//normal, diffuse, position, tex
	GLuint m_textures[4];
	GLuint m_depthTexture;
	GLuint renderedTexture;
	GLuint depthrenderbuffer;

	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_TEXCOORD,
		GBUFFER_NUM_TEXTURES
	};
};

#endif
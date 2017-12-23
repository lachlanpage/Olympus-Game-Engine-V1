#ifndef RENDERER_H
#define RENDERER_H
#include <glad/glad.h>
class Renderer {
public:
	static Renderer* m_Instance;
	static Renderer* Instance();
	void render(GLenum, GLint, GLsizei);
	
private:
	Renderer();
};

#endif
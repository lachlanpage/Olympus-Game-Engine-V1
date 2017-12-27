#ifndef GRAPHICS_H
#define GRAPHICS_H 
#include "../core/Entity.h"
#include "../core/Renderer.h"
class Entity;

class GraphicsComponent {
public:
	virtual void update(Entity& entity) = 0;
protected:
	void sendToRenderer(GLenum mode, GLint first, GLsizei count){ Renderer::Instance()->render(mode, first, count); }
};

#endif
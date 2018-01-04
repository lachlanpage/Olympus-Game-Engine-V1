#ifndef COMPONENT_H
#define COMPONENT_H
#include "../core/Entity.h"
#include "../core/Renderer.h"
#include <iostream>
class Entity;

class Component {
public:
	virtual void update(Entity& entity) = 0;
	virtual void postInit(Entity& entity) = 0;
	virtual void renderShadow(Entity& entity){}
protected:
	void sendToRenderer(GLenum mode, GLint first, GLsizei count) { Renderer::Instance()->render(mode, first, count); }

};

#endif
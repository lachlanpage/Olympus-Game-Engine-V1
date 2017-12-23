#ifndef GRAPHICS_H
#define GRAPHICS_H 
#include "../core/Entity.h"

class Entity;

class GraphicsComponent {
public:
	virtual void update(Entity& entity) = 0;
};

#endif

#ifndef COMPONENT_H
#define COMPONENT_H


enum Component : unsigned char
{
	kComponentBase,
	kComponentSample,
	kComponentTransform,
	kComponentDraw,
	kComponentPhysics,
	kComponentCamera,
	kComponentCollision,
	kComponentEnd
};

class BaseComponent;


#endif

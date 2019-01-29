
#ifndef COMPONENT_H
#define COMPONENT_H


enum Component : unsigned char
{
	kComponentBase,
	kComponentSample,
	kComponentTransform,
	kComponentDraw,
	kComponentPhysics,
	kComponentEnd
};

class BaseComponent;


#endif

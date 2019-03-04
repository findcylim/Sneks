
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
	KComponentInvulnerable,
	kComponentSnekHead,
	kComponentFollow,
	kComponentParticleEffect,
	kComponentParticle,
	kComponentCanvas,
	kComponentCanvasElement,
	kComponentTextRenderer,
	kComponentEnd
};

class BaseComponent;


#endif


#ifndef COMPONENT_H
#define COMPONENT_H


enum Component : unsigned char
{
	kComponentBase,
	kComponentTransform,
	kComponentDraw,
	kComponentAnimation,
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
	kComponentPowerUp,
	kComponentEnd
};

class BaseComponent;


#endif

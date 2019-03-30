
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
	kComponentBloom,
	kComponentPowerUpHolder,
	kComponentEnd
};

class BaseComponent;


#endif


#ifndef COMPONENT_H
#define COMPONENT_H


enum Component : unsigned char
{
	kComponentBase,
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
	kComponentPowerUp,
	kComponentEnd
};

class BaseComponent;


#endif

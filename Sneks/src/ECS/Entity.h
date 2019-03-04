
#ifndef ENTITY_H
#define ENTITY_H


enum Entity : unsigned char
{
	kEntityBase,
	kEntitySnekHead,
	kEntitySnekBody,
	kEntitySnekTail,
	kEntityMoon,
	kEntityStaticObject,
	kEntityBackground,
	kEntityCamera,
	kEntityProjectile,
	kEntityParticleEffect,
	kEntityParticle,
	kEntityPowerUpHolder,
	kEntityEnd
};

class BaseEntity;


#endif

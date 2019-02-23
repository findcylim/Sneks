
#ifndef ENTITY_H
#define ENTITY_H


enum Entity : unsigned char
{
	kEntityBase,
	kEntitySample,
	kEntitySnekHead,
	kEntitySnekBody,
	kEntitySnekTail,
	kEntityMoon,
	kEntityStaticObject,
	kEntityBackground,
	kEntityCamera,
	kEntityProjectile,
	kEntityEnd
};

class BaseEntity;


#endif


#ifndef ENTITY_H
#define ENTITY_H


enum Entity : unsigned char
{
	kEntityBase,
	kEntitySample,
	kEntitySnekHead,
	kEntitySnekBody,
	kEntityStaticObject,
	kEntityBackground,
	kEntityCamera,
	kEntityEnd
};

class BaseEntity;


#endif

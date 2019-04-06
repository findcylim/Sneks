/* Start Header****************************************************************/
/*!
\file Entity.h
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains the list of all entities archetypes we have

\par Contribution : CY     - 23.08%
\par Contribution : Javier - 23.08%
\par Contribution : Edwin  - 26.92%
\par Contribution : Adam   - 26.92% 

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/


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
	kEntityCanvas,
	kEntityCanvasButton,
	kEntityCanvasBasicSprite,
	kEntityCanvasTextLabel,
	kEntityMouse,
	kEntityPowerUpHolder,
	kEntityScreenOverlay,
	kEntityEnd
};

class BaseEntity;


#endif

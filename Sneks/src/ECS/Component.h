/* Start Header****************************************************************/
/*!
\file Component.h
\author Primary Author : Edwin Khoo, b.khoo
\par email: b.khoo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : Edwin  - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/


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
	kComponentTint,
	kComponentEnd
};

class BaseComponent;


#endif

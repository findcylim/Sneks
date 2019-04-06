/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

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

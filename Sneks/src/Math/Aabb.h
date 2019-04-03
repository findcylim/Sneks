/* Start Header ***************************************************************/
/*!
\file Aabb.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains the code for static aabb x aabb collision checks.

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef AABB_H
#define AABB_H
#pragma once
#include "../Math/HTVector2.h"

struct Aabb
{
	HTVector2 min;
	HTVector2 max;

	bool operator ==(const Aabb& rhs) const
	{
		return (min == rhs.min && max == rhs.max);
	}
	bool operator !=(const Aabb& rhs) const
	{
		return !(*this == rhs);
	}

};

namespace AabbHelper
{
	bool CheckAabbIntersect(const Aabb&, const Aabb&);
  /**
   * \brief Gets the bottom left corner of the AABB for specified position,size
   * and scale.
   */
	HTVector2 GetMin(const HTVector2 position, const HTVector2 size, const float scale);
    /**
   * \brief Gets the bottom left corner of the AABB for specified position,size
   * and scale.
   */
	HTVector2 GetMax(const HTVector2 position, const HTVector2 size, const float scale);
    /**
   * \brief Gets the top right corner of the AABB for specified position,size
   * and scale.
   */
	HTVector2 GetMin(const HTVector2 position, const HTVector2 size, const HTVector2 scale);
    /**
   * \brief Gets the top right corner of the AABB for specified position,size
   * and scale.
   */
	HTVector2 GetMax(const HTVector2 position, const HTVector2 size, const HTVector2 scale);
}
#endif //AABB_H
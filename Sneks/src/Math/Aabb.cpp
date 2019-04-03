/* Start Header ***************************************************************/
/*!
\file Aabb.cpp
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

#include "Aabb.h"

namespace AabbHelper {
  /**
   * \brief Static AABB collision check
   * 
   */
	bool CheckAabbIntersect(const Aabb& a, const Aabb& b)
	{
		if (b.min.x > a.max.x)
			return false;
		if (b.min.y > a.max.y)
			return false;
		if (a.min.x > b.max.x)
			return false;
		if (a.min.y > b.max.y)
			return false;
		return true;
	}

  /**
   * \brief Subtracts half the scale * mesh size to the position to get the rectangle.
   * 
   * \param position Position
   * \param size Mesh Size
   * \param scale  Draw Scale
   * \return HTVector2 
   */
	HTVector2 GetMin(const HTVector2 position, const HTVector2 size, const float scale)
	{
		HTVector2 min ={};
		min.x = position.x - size.x * scale / 2;
		min.y = position.y - size.y * scale / 2;
		return min;
	}

  /**
   * \brief Subtracts half the scale * mesh size to the position to get the rectangle.
   * 
   * \param position Position
   * \param size Mesh Size
   * \param scale  Draw Scale
   * \return HTVector2 
   */
	HTVector2 GetMin(const HTVector2 position, const HTVector2 size, const HTVector2 scale)
	{
		HTVector2 min ={};
		min.x = position.x - size.x * scale.x / 2;
		min.y = position.y - size.y * scale.y / 2;
		return min;
	}

  /**
   * \brief Adds half the scale * mesh size to the position to get the rectangle.
   * 
   * \param position Position
   * \param size Mesh Size
   * \param scale  Draw Scale
   * \return HTVector2 
   */
	HTVector2 GetMax(const HTVector2 position, const HTVector2 size, const float scale)
	{
		HTVector2 max ={};
		max.x = position.x + size.x * scale / 2;
		max.y = position.y + size.y * scale / 2;
		return max;
	}

  /**
   * \brief Adds half the scale * mesh size to the position to get the rectangle.
   * 
   * \param position Position
   * \param size Mesh Size
   * \param scale  Draw Scale
   * \return HTVector2 
   */
	HTVector2 GetMax(const HTVector2 position, const HTVector2 size, const HTVector2 scale)
	{
		HTVector2 max ={};
		max.x = position.x + size.x * scale.x / 2;
		max.y = position.y + size.y * scale.y / 2;
		return max;
	}

}
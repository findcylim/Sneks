/* Start Header ***************************************************************/
/*!
\file HTVector2.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 5

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#pragma once
struct HTVector2 final
{
	float x;
	float y;

	float Dot(const HTVector2& rhs) const
	{
		return rhs.x * x + rhs.y * y;
	}

	HTVector2& operator-=(const HTVector2& rhs)
	{
		x = x - rhs.x;
		y = y - rhs.y;
		return *this;
	}
	HTVector2& operator+=(const HTVector2& rhs)
	{
		x = x + rhs.x;
		y = y + rhs.y;
		return *this;
	}

	HTVector2 operator-(const HTVector2& rhs) const
	{
		HTVector2 result;
		result.x = x - rhs.x;
		result.y = y - rhs.y;
		return result;
	}
	HTVector2 operator+(const HTVector2& rhs) const
	{
		HTVector2 result;
		result.x = x + rhs.x;
		result.y = y + rhs.y;
		return result;
	}

	HTVector2 operator*(const float& rhs) const
	{
		HTVector2 result;
		result.x = x * rhs;
		result.y = y * rhs;
		return result;
	}

	HTVector2 operator*(const HTVector2& rhs) const
	{
		HTVector2 result;
		result.x = x * rhs.x;
		result.y = y * rhs.y;
		return result;
	}

	HTVector2 operator/(const float& rhs) const
	{
		HTVector2 result;
		result.x = x / rhs;
		result.y = y / rhs;
		return result;
	}

	HTVector2& operator=(const HTVector2& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	HTVector2& operator=(const float rhs)
	{
		x = rhs;
		y = rhs;
		return *this;
	}

	bool operator==(const HTVector2& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	bool operator!=(const HTVector2& rhs) const
	{
		return !(*this == rhs);
	}
};


inline HTVector2 CalculateReflectVelocity(HTVector2 velocity, HTVector2 wallNormal)
{
	HTVector2 perpendicular = wallNormal * ( velocity.Dot(wallNormal) / wallNormal.Dot(wallNormal)) ;
	HTVector2 parallel		= velocity - perpendicular;

	auto newVel = parallel - perpendicular;

	AEVec2 vel{ newVel.x,newVel.y };
	if (AEVec2SquareLength(&vel) < 300.0f * 300)
	{
		AEVec2Normalize(&vel, &vel);
		newVel ={ vel.x * 300.0f, vel.y * 300.0f };
	}

	return newVel;
}

inline HTVector2 GetNormal(HTVector2 vector)
{
	HTVector2 result;
	result.x = vector.y * -1;
	result.y = vector.x;
	return result;
}


namespace MathHT
{

	inline float Abs(float value)
	{
		return value < 0 ? -value : value;
	}

	inline float CalculateVector2Distance(HTVector2 lhs, HTVector2 rhs)
	{
		return (Abs((lhs.x - rhs.x)*(lhs.x - rhs.x)) + Abs((lhs.y - rhs.y)*(lhs.y - rhs.y)));
	}

	inline float CalculateDistanceX(HTVector2 lhs, HTVector2 rhs)
	{
		return Abs(lhs.x - rhs.x);
	}

	inline float CalculateDistanceY(HTVector2 lhs, HTVector2 rhs)
	{
		return Abs(lhs.y - rhs.y);
	}
}


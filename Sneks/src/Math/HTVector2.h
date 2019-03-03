#pragma once
struct HTVector2 final
{
	float x;
	float y;

	float dot(const HTVector2& rhs) const
	{
		return rhs.x * x + rhs.y * y;
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
	
};


inline HTVector2 CalculateReflectVelocity(HTVector2 velocity, HTVector2 wallNormal)
{
	HTVector2 perpendicular = wallNormal * ( velocity.dot(wallNormal) / wallNormal.dot(wallNormal)) ;
	HTVector2 parallel		= velocity - perpendicular;
	return parallel - perpendicular;
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


#pragma once
struct HTVector2 final
{
	float x;
	float y;
	HTVector2 operator+(const HTVector2& toAdd) const
	{
		HTVector2 result;
		result.x = x + toAdd.x;
		result.y = y + toAdd.y;
		return result;
	}
	
};



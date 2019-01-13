#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H
#pragma once

#include "AEEngine.h"


class DrawObject {
protected:
	float		      m_f_Rotation = 0;
	float			  m_f_Velocity = 0;
	float			  m_f_PositionX;
	float			  m_f_PositionY;
	float			  m_f_SizeX;
	float			  m_f_SizeY;
	AEGfxTexture*	  m_px_Texture;	
	AEGfxVertexList*  m_px_Obj;

public:
	void SetTexture(AEGfxTexture * tex);
	void SetRotation(float f);
	void SetVelocity(float f);
	void SetPositionX(float f);
	void SetPositionY(float f);
	float GetRotation() const;
	float GetVelocity() const;
	float GetPositionX() const;
	float GetPositionY() const;
	float GetSizeX()	 const;
	float GetSizeY()	 const;
	DrawObject(float posX, float posY, float sizeX, float sizeY, AEGfxTexture* tex);
	~DrawObject(void);
	void Draw();
	void Update();
	float GetRotatedOffsetXx() const;
	float GetRotatedOffsetXy() const;
	float GetRotatedOffsetYx() const;
	float GetRotatedOffsetYy() const;

};

#endif	//DRAW_OBJECT_H
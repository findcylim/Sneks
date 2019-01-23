#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H
#pragma once

#include "Aabb.h"
#include "AEEngine.h"

class DrawObject {
protected:
	float		               m_f_Rotation = 0;
	float			            m_f_Velocity = 0;
	//float			            m_x_Position.x;
	//float			            m_x_Position.y;
	Vector2						m_x_Position;
	float			            m_f_SizeX;
	float			            m_f_SizeY;
	int							m_f_RgbaColor = 9999;
	AEGfxTexture*	         m_px_Texture;
	AEGfxVertexList*        m_px_Obj;
	AEMtx33*				      m_po_RotationMatrix;
	AEMtx33*		            m_po_TranslationMatrix;
	AEMtx33*                m_po_GlobalMatrix;
	float							m_f_Scale = 1.0f;

public:
	static float				m_f_GlobalScale;
	int GetColor() const;
	float GetScale() const;
	void SetScale(float f);
	void SetColor(int rgba);
	void SetTexture(AEGfxTexture * tex);
	void SetRotation(float f);
	void SetVelocity(float f);
	void SetPositionX(float f);
	void SetPositionY(float f);
	Vector2 GetMin(void) const;
	Vector2 GetMax(void) const;
	float GetRotation() const;
	float GetVelocity() const;
	Vector2 GetPosition() const;
	float GetSizeX()	 const;
	float GetSizeY()	 const;
	DrawObject(float posX, float posY, float sizeX, float sizeY, AEGfxTexture* tex);
	~DrawObject(void);
	void Draw();
	float GetRotatedOffsetXx() const;
	float GetRotatedOffsetXy() const;
	float GetRotatedOffsetYx() const;
	float GetRotatedOffsetYy() const;
};

#endif	//DRAW_OBJECT_H
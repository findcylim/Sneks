#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H
#pragma once

#include "../Math/Aabb.h"
#include "AEEngine.h"
#include "HTVector2.h"
#include "HTColor.h"

class DrawObject {
protected:
	float		               m_f_Rotation = 0;
	//float			            m_f_VelocityX = 0;
	//float			            m_f_VelocityY = 0;
	float							m_f_Velocity;
	HTVector2					m_x_Acceleration;
	//float							m_f_AccelerationX = 0;
	//float							m_f_AccelerationY = 0;
	//float			            m_x_Position.x;
	//float			            m_x_Position.y;
	HTVector2					m_x_Position;
	HTVector2					m_x_Size;
	//float			            m_f_SizeX;
	//float			            m_f_SizeY;
	HTColor						m_f_RgbaColor = {1,1,1,1};
	AEGfxTexture*	         m_px_Texture;
	AEGfxVertexList*        m_px_Obj;
	AEMtx33*				      m_po_RotationMatrix;
	AEMtx33*		            m_po_TranslationMatrix;
	AEMtx33*                m_po_GlobalMatrix;
	float							m_f_Scale = 1.0f;

public:
	static float				m_f_GlobalScale;
	HTColor GetColor() const;
	float GetScale() const;
	void SetScale(float f);
	void SetColor(float red, float green, float blue, float alpha);
	void SetAlpha(float alpha);
	void SetTexture(AEGfxTexture * tex);
	void SetRotation(float f);
	void SetVelocity(const float);
	void SetPosition(float,float);
	//void SetPositionX(float f);
	//void SetPositionY(float f);
	HTVector2 GetMin(void) const;
	HTVector2 GetMax(void) const;
	float GetRotation() const;
	float GetVelocity() const;
	HTVector2 GetPosition() const;
	float GetSizeX()	 const;
	float GetSizeY()	 const;
	DrawObject(float posX, float posY, float sizeX, float sizeY, AEGfxTexture* tex);
	~DrawObject(void);
	void Draw();
	float GetRotatedOffsetXx() const;
	float GetRotatedOffsetXy() const;
	float GetRotatedOffsetYx() const;
	float GetRotatedOffsetYy() const;
	HTVector2 ApplyVelocity(float dt);
	HTVector2 GetForwardVelocity();
};

#endif	//DRAW_OBJECT_H
#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H
#pragma once

#include "AEEngine.h"
#include "../Math/HTVector2.h"
#include "../Math/HTColor.h"
#include "Camera.h"


class DrawObject {
protected:
	float		               m_f_Rotation;
	float							m_f_Velocity;
	HTVector2					m_x_Acceleration;
	HTVector2					m_x_Position;
	HTVector2					m_x_Size;
	HTColor						m_f_RgbaColor;
	AEGfxTexture*	         m_px_Texture;
	AEGfxVertexList*        m_px_Obj;
	AEMtx33*				      m_po_RotationMatrix;
	AEMtx33*		            m_po_TranslationMatrix;
	AEMtx33*                m_po_GlobalMatrix;
	float							m_f_Scale;

public:


	AEGfxVertexList* GetMesh() const;
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
	DrawObject(float posX, float posY, float sizeX, float sizeY, AEGfxTexture* tex, AEGfxVertexList* mesh);
	~DrawObject(void);
	void Draw(Camera*) const;
	float GetRotatedOffsetXx() const;
	float GetRotatedOffsetXy() const;
	float GetRotatedOffsetYx() const;
	float GetRotatedOffsetYy() const;
	HTVector2 ApplyVelocity(float dt);
	HTVector2 GetForwardVelocity() const;
};

#endif	//DRAW_OBJECT_H
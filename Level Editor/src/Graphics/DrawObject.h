#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H
#pragma once

#include "../Math/Aabb.h"
#include "AEEngine.h"
#include "../Math/HTVector2.h"
#include "../Math/HTColor.h"


class DrawObject {
protected:
	float		               m_f_Rotation = 0;
	float							m_f_Velocity;
	HTVector2					m_x_Acceleration;
	HTVector2					m_x_Position;
	HTVector2					m_x_Size;
	HTColor						m_f_RgbaColor = {1,1,1,1};
	AEGfxTexture*	         m_px_Texture;
	AEGfxVertexList*        m_px_Obj;
	AEMtx33*				      m_po_RotationMatrix;
	AEMtx33*		            m_po_TranslationMatrix;
	AEMtx33*                m_po_GlobalMatrix;
	float							m_f_ScaleX = 1.0f;
	float							m_f_ScaleY = 1.0f;
	char*					m_c_Name;
	char*					m_c_TextureName;
public:
	static float				m_f_GlobalScale;
	static float				m_f_GlobalCameraOffsetX;
	static float				m_f_GlobalCameraOffsetY;

	HTColor GetColor() const;
	float GetScaleX() const;
	float GetScaleY() const;
	void SetScale(float f);
	void SetScale(float x,float y);
	void SetColor(float red, float green, float blue, float alpha);
	void SetAlpha(float alpha);
	void SetTexture(AEGfxTexture * tex);
	void SetRotation(float f);
	void SetVelocity(const float);
	void SetPosition(float,float);
	void SetPositionX(float f);
	void SetPositionY(float f);
	void SetSizeX(float f);
	void SetSizeY(float f);
	HTVector2 GetMin(void) const;
	HTVector2 GetMax(void) const;
	float GetRotation() const;
	float GetVelocity() const;
	HTVector2 GetPosition() const;
	float GetSizeX()	 const;
	float GetSizeY()	 const;
	DrawObject(float posX, float posY, float sizeX, float sizeY, struct AEGfxTexture* tex, const char * name, const char * texName);
	DrawObject(const DrawObject& drawObj);
	~DrawObject(void);
	void Draw();
	float GetRotatedOffsetXx() const;
	float GetRotatedOffsetXy() const;
	float GetRotatedOffsetYx() const;
	float GetRotatedOffsetYy() const;
	HTVector2 ApplyVelocity(float dt);
	HTVector2 GetForwardVelocity();

	const char* GetName()	const;
	const char* GetTexName() const;
};

#endif	//DRAW_OBJECT_H
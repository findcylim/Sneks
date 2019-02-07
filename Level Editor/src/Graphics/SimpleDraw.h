#ifndef SIMPLE_DRAW_H
#define SIMPLE_DRAW_H
#pragma once

#include "../Math/Aabb.h"
#include "AEEngine.h"
#include "../Math/HTVector2.h"
#include "../Math/HTColor.h"


class SimpleDraw {
protected:
	float		               m_f_Rotation = 0;
	float							m_f_Velocity;
	HTVector2					m_x_Acceleration;
	HTVector2					m_x_Position;
	HTVector2					m_x_Size;
	HTVector2					m_x_Scale;
	HTColor						m_f_RgbaColor = {1,1,1,1};
	AEGfxVertexList*        m_px_Obj;
	AEMtx33*				      m_po_RotationMatrix;
	AEMtx33*		            m_po_TranslationMatrix;
	AEMtx33*                m_po_GlobalMatrix;
	char*					m_c_Name;
public:
	static float				m_f_GlobalScale;
	static float				m_f_GlobalCameraOffsetX;
	static float				m_f_GlobalCameraOffsetY;

	HTColor GetColor() const;
	HTVector2 GetScale() const;
	void SetScale(float x,float y);
	void SetColor(float red, float green, float blue, float alpha);
	void SetAlpha(float alpha);
	void SetRotation(float f);
	void SetVelocity(const float);
	void SetPosition(float,float);
	void SetPositionX(float f);
	void SetPositionY(float f);
	HTVector2 GetMin(void) const;
	HTVector2 GetMax(void) const;
	float GetRotation() const;
	float GetVelocity() const;
	HTVector2 GetPosition() const;
	float GetSizeX()	 const;
	float GetSizeY()	 const;
	SimpleDraw(float posX, float posY, float sizeX, float sizeY, const char * name);
	SimpleDraw(const SimpleDraw& drawObj);
	~SimpleDraw(void);
	void Draw();
	float GetRotatedOffsetXx() const;
	float GetRotatedOffsetXy() const;
	float GetRotatedOffsetYx() const;
	float GetRotatedOffsetYy() const;
	HTVector2 ApplyVelocity(float dt);
	HTVector2 GetForwardVelocity();
	const char* GetName()	const;
};

#endif	//DRAW_OBJECT_H
#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H
#pragma once

#include "../Math/Aabb.h"
#include "AEEngine.h"

class DrawObject {
protected:
	float		            m_f_Rotation = 0;
	float					m_f_Velocity = 0;
	float			        m_x_PositionX;
	float			        m_x_PositionY;
	Vector2					m_x_Position;
	float			        m_f_SizeX;
	float			        m_f_SizeY;
	int						m_f_RgbaColor = 9999;
	struct AEGfxTexture*	m_px_Texture;
	AEGfxVertexList*        m_px_Obj;
	AEMtx33*				m_po_RotationMatrix;
	AEMtx33*		        m_po_TranslationMatrix;
	AEMtx33*                m_po_GlobalMatrix;
	char*					m_c_Name;
	char*					m_c_TextureName;
public:
	void SetColor(int rgba);
	void SetTexture(struct AEGfxTexture * tex);
	void SetRotation(float f);
	void SetVelocity(float f);
	void SetPositionX(float f);
	void SetPositionY(float f);
	Vector2 GetMin(void);
	Vector2 GetMax(void);
	float GetRotation() const;
	float GetVelocity() const;
	Vector2 GetPosition() const;
	float GetSizeX()	 const;
	float GetSizeY()	 const;
	const char* GetName()	const;
	const char* GetTexName() const;
	AEGfxVertexList* GetVertexList()const;
	struct AEGfxTexture* GetTexture() const;
	DrawObject(float posX, float posY, float sizeX, float sizeY, struct AEGfxTexture* tex,const char * name,const char * texName);
	DrawObject(const DrawObject& drawObj);
	~DrawObject(void);
	void Draw();
	void Update();
	float GetRotatedOffsetXx() const;
	float GetRotatedOffsetXy() const;
	float GetRotatedOffsetYx() const;
	float GetRotatedOffsetYy() const;
	void SetScale(float f);
};

#endif	//DRAW_OBJECT_H
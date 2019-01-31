#ifndef DRAWCOMPONENT_H
#define DRAWCOMPONENT_H

#pragma once


#include "AEEngine.h"
#include "../Math/HTVector2.h"
#include "../ECS/ComponentList.h"
#include "../Math/HTColor.h"
#include "TransformComponent.h"

class DrawComponent : public BaseComponent
{
	HTColor					   m_f_RgbaColor = { 1,1,1,1 };
	AEGfxTexture*	         m_px_Texture;
	AEGfxVertexList*        m_px_Obj;
	AEMtx33*				      m_po_RotationMatrix;
	AEMtx33*		            m_po_TranslationMatrix;
	AEMtx33*                m_po_GlobalMatrix;
	HTVector2				   m_x_Size;
	TransformComponent*		m_po_TransformComponent;


public:

	static float				m_f_GlobalScale;
	static float				m_f_GlobalCameraOffsetX;
	static float				m_f_GlobalCameraOffsetY;
	DrawComponent() = delete;
	DrawComponent(BaseComponent* transformComponent);
	AEGfxVertexList* GetMesh() const;
	HTColor GetColor() const;
	void SetColor(float red, float green, float blue, float alpha);
	void SetAlpha(float alpha);
	void SetTexture(AEGfxTexture * tex);
	float GetSizeX()	 const;
	float GetSizeY()	 const;
};

#endif
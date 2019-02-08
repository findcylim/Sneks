#ifndef DRAW_COMPONENT_H
#define DRAW_COMPONENT_H

#pragma once


#include "AEEngine.h"
#include "../Math/HTVector2.h"
#include "../ECS/ComponentList.h"
#include "../Math/HTColor.h"
#include "TransformComponent.h"

class DrawComponent : public BaseComponent
{
public:
	AEGfxTexture*	            m_px_Texture;
	AEGfxVertexList*		      m_px_Mesh;
	HTVector2				      m_x_MeshSize;
	HTColor					      m_f_RgbaColor ={ 1,1,1,1 };

	TransformComponent*		   m_po_TransformComponent;

	AEMtx33*				         m_po_RotationMatrix;
	AEMtx33*		               m_po_TranslationMatrix;
	AEMtx33*				         m_po_GlobalMatrix;


	void SetAlpha(float alpha);
	void SetColor(float red, float green, float blue, float alpha);
	float GetSizeX()	 const;
	float GetSizeY()	 const;
	void Initialize(AEGfxTexture* texture, float sizeX, float sizeY, HTColor color);
};

#endif
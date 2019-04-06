/* Start Header****************************************************************/
/*!
\file DrawComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef DRAW_COMPONENT_H
#define DRAW_COMPONENT_H

#pragma once


#include "AEEngine.h"
#include "../Math/HTVector2.h"
#include "../ECS/ComponentList.h"
#include "../Math/HTColor.h"
#include "TransformComponent.h"

struct DrawComponent : public BaseComponent
{
public:
	AEGfxTexture*	            m_px_Texture;
	AEGfxVertexList*		      m_px_Mesh;
	HTVector2				      m_x_MeshSize;
	HTColor					      m_f_RgbaColor ={ 1,1,1,1 };
	HTVector2						m_x_TextureOffset ={ 0 };
	TransformComponent*		   m_po_TransformComponent;
	AEMtx33*				         m_po_RotationMatrix;
	AEMtx33*		               m_po_TranslationMatrix;
	AEMtx33*				         m_po_GlobalMatrix;
	AEMtx33*				         m_po_ScaleMatrix;
	int							   m_f_DrawPriority = 5;

	DrawComponent();
	~DrawComponent() override;
	void SetAlpha(float alpha);
	float GetAlpha() const;
	HTVector2 GetSizeInPixels() const;
};

#endif
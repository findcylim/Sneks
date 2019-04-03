/* Start Header ***************************************************************/
/*!
\file DrawComponent.cpp
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

#include "DrawComponent.h"
#include "../Utility/FileIO.h"


DrawComponent::DrawComponent()
{
	m_po_GlobalMatrix = new AEMtx33();
	m_po_RotationMatrix = new AEMtx33();
	m_po_TranslationMatrix = new AEMtx33();
	m_po_ScaleMatrix = new AEMtx33();
}

DrawComponent::~DrawComponent()
{
	delete m_po_GlobalMatrix;
	delete m_po_RotationMatrix;
	delete m_po_TranslationMatrix;
	delete m_po_ScaleMatrix;
}

void DrawComponent::SetAlpha(float alpha)
{
	m_f_RgbaColor.alpha = alpha;
}

float DrawComponent::GetAlpha() const
{
	return m_f_RgbaColor.alpha;
}

void DrawComponent::SetColor(float red, float green, float blue, float alpha)
{
	m_f_RgbaColor.red = red;
	m_f_RgbaColor.green = green;
	m_f_RgbaColor.blue = blue;
	m_f_RgbaColor.alpha = alpha;
}

float DrawComponent::GetMeshSizeX() const
{
	return m_x_MeshSize.x;
}

float DrawComponent::GetMeshSizeY() const
{
	return m_x_MeshSize.y;
}

HTVector2 DrawComponent::GetSizeInPixels() const
{
	return m_po_TransformComponent->m_f_Scale * m_po_TransformComponent->m_f_ScaleMultiplier * m_x_MeshSize;
}

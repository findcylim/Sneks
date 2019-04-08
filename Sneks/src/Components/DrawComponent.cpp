/* Start Header****************************************************************/
/*!
\file DrawComponent.cpp
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains some data manipulation function to
facilitate usage of the draw component. Most work should be done in graphics
system though.

\par Contribution : CY - 100.00%

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


HTVector2 DrawComponent::GetSizeInPixels() const
{
	return m_po_TransformComponent->m_f_Scale * m_po_TransformComponent->m_f_ScaleMultiplier * m_x_MeshSize;
}

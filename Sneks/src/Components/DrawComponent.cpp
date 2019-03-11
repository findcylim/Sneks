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

float DrawComponent::GetSizeX() const
{
	return m_x_MeshSize.x;
}

float DrawComponent::GetSizeY() const
{
	return m_x_MeshSize.y;
}

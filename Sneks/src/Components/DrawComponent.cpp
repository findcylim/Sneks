#include "DrawComponent.h"


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

void DrawComponent::Initialize(AEGfxTexture* texture, AEGfxVertexList* mesh, const float sizeX, const float sizeY, HTColor color = {1,1,1,1} )
{
	m_px_Texture  = texture;
	m_px_Mesh     = mesh;
	m_x_MeshSize  = {sizeX, sizeY};
	m_f_RgbaColor = color;
}
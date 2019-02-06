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

void DrawComponent::Initialize(AEGfxTexture* texture, const float sizeX, const float sizeY, HTColor color = {1,1,1,1} )
{
	m_px_Texture  = texture;

	AEGfxMeshStart();
	AEGfxTriAdd(-(sizeX / 2), -(sizeY / 2), 0x00FFFFFF, 0, 1,
		sizeX / 2, -(sizeY / 2), 0x0000FFFF, 1, 1,
		-(sizeX / 2), sizeY / 2, 0x00FFFF00, 0, 0);

	AEGfxTriAdd(
		sizeX / 2, sizeY / 2, 0x00FFFFFF, 1, 0,
		-(sizeX / 2), sizeY / 2, 0x00FFFFFF, 0, 0,
		sizeX / 2, -(sizeY / 2), 0x00FFFFFF, 1, 1);
	m_px_Mesh = AEGfxMeshEnd();

	m_x_MeshSize  = {sizeX, sizeY};
	m_f_RgbaColor = color;

	m_po_GlobalMatrix = new AEMtx33();
	m_po_RotationMatrix = new AEMtx33();
	m_po_TranslationMatrix = new AEMtx33();
}
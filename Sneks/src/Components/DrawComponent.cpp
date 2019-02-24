#include "DrawComponent.h"
#include "../Utility/FileIO.h"


void DrawComponent::SetAlpha(float alpha)
{
	m_f_RgbaColor.alpha = alpha;
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
/*
void DrawComponent::Initialize(AEGfxTexture* texture, const float sizeX, const float sizeY, HTColor color )
{
	m_px_Texture  = texture;
	auto largerDimension = max(sizeX, sizeY);

	auto normalizedX = sizeX / largerDimension;
	auto normalizedY = sizeY / largerDimension;

	m_x_MeshSize  ={ normalizedX, normalizedY };

	//check if mesh exists
	AEGfxMeshStart();
	AEGfxTriAdd(-(normalizedX / 2), -(normalizedY / 2), 0x00FFFFFF, 0, 1,
		normalizedX / 2, -(normalizedY / 2), 0x0000FFFF, 1, 1,
		-(normalizedX / 2), normalizedY / 2, 0x00FFFF00, 0, 0);

	AEGfxTriAdd(
		normalizedX / 2, normalizedY / 2, 0x00FFFFFF, 1, 0,
		-(normalizedX / 2), normalizedY / 2, 0x00FFFFFF, 0, 0,
		normalizedX / 2, -(normalizedY / 2), 0x00FFFFFF, 1, 1);
	m_px_Mesh = AEGfxMeshEnd();

	m_po_TransformComponent->m_f_Scale *= largerDimension;

	m_f_RgbaColor = color;

	m_po_GlobalMatrix = new AEMtx33();
	m_po_RotationMatrix = new AEMtx33();
	m_po_TranslationMatrix = new AEMtx33();
}

void DrawComponent::Initialize(AEGfxTexture* texture, HTColor color)
{
	int sizeX, sizeY;
	FileIO::ReadPngDimensions(texture->mpName, &sizeX, &sizeY);
	Initialize(texture, static_cast<float>(sizeX), static_cast<float>(sizeY), color);
}
*/
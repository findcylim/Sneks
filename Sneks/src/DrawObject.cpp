#include "DrawObject.h"
#include <cmath>
#include "AEMtx33.h"
#include "AEVec2.h"
#include "AEGraphics.h"


float DrawObject::GetScale()
{
	return m_f_Scale;
}

void DrawObject::SetScale(float f)
{
	m_f_Scale = f;
}
void DrawObject::SetColor(int rgba)
{
	m_f_RgbaColor = rgba;
}

void DrawObject::SetTexture(AEGfxTexture* tex) {
	m_px_Texture = tex;
}
void DrawObject::SetRotation(const float f) {
	m_f_Rotation = f;
}
void DrawObject::SetVelocity(const float f) {
	m_f_Velocity = f;
}
void DrawObject::SetPositionX(const float f) {
	m_x_Position.x = f;
}
void DrawObject::SetPositionY(const float f) {
	m_x_Position.y = f;
}

Vector2 DrawObject::GetMin()
{
	Vector2 min = {};
	min.x = m_x_Position.x - m_f_SizeX / 2;
	min.y = m_x_Position.y - m_f_SizeY / 2;
	return min;
}

Vector2 DrawObject::GetMax()
{
	Vector2 max = {};
	max.x = m_x_Position.x + m_f_SizeX / 2;
	max.y = m_x_Position.y + m_f_SizeY / 2;
	return max;
}

float DrawObject::GetRotation() const
{
	return m_f_Rotation;
}
float DrawObject::GetVelocity() const
{
	return m_f_Velocity;
}
Vector2 DrawObject::GetPosition() const
{
	return m_x_Position;
}


float DrawObject::GetSizeX() const
{
	return m_f_SizeX;
}

float DrawObject::GetSizeY() const
{
	return m_f_SizeY;
}


DrawObject::DrawObject(const float posX, const float posY, const float sizeX, const float sizeY, AEGfxTexture* tex)
{
	SetPositionX(posX);
	SetPositionY(posY);
	m_px_Texture  = tex;
	m_f_SizeX     = sizeX;
	m_f_SizeY     = sizeY;
	AEGfxMeshStart();
	AEGfxTriAdd(-(sizeX/2), -(sizeY/2), 0x00FFFFFF, 0, 1,
		sizeX / 2, -(sizeY/2), 0x0000FFFF, 1, 1,
		-(sizeX / 2), sizeY / 2, 0x00FFFF00, 0, 0);

	AEGfxTriAdd(
		sizeX / 2, sizeY / 2, 0x00FFFFFF, 1, 0,
		-(sizeX / 2), sizeY / 2, 0x00FFFFFF, 0, 0,
		sizeX / 2, -(sizeY / 2), 0x00FFFFFF, 1, 1);
	m_px_Obj = AEGfxMeshEnd();

	m_po_GlobalMatrix      = new AEMtx33();
	m_po_RotationMatrix    = new AEMtx33();
	m_po_TranslationMatrix = new AEMtx33();

}

DrawObject::~DrawObject(void)
{
	delete m_po_GlobalMatrix;
	delete m_po_TranslationMatrix;
	delete m_po_RotationMatrix;

}

void DrawObject::Draw() {
	
	AEMtx33Rot(m_po_RotationMatrix, m_f_Rotation);
	AEMtx33ScaleApply(m_po_RotationMatrix, m_po_RotationMatrix, m_f_Scale, m_f_Scale);
	AEMtx33Trans(m_po_TranslationMatrix, m_x_Position.x, m_x_Position.y);
	/*generate global matrix from rot and trans*/
	AEMtx33Concat(m_po_GlobalMatrix, m_po_TranslationMatrix, m_po_RotationMatrix);

	//allow transparency to work !!must be first
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	float red = (m_f_RgbaColor / 1000) * 0.11f;
	float green = (m_f_RgbaColor / 100 % 10) * 0.11f;
	float blue = (m_f_RgbaColor / 10 % 10) * 0.11f;
	float alpha = (m_f_RgbaColor % 10) * 0.11f;

	AEGfxSetTintColor(red, green, blue, alpha);
	AEGfxTextureSet(m_px_Texture, 0, 0);
	AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	AEGfxSetTransparency(1);
	AEGfxSetPosition(m_x_Position.x, m_x_Position.y);
	AEGfxSetTransform(m_po_GlobalMatrix->m);
	AEGfxMeshDraw(m_px_Obj, AE_GFX_MDM_TRIANGLES);
}

float DrawObject::GetRotatedOffsetXx() const
{
	return m_f_SizeX / 2 * static_cast<float>(std::cos(GetRotation())) ;
}
float DrawObject::GetRotatedOffsetXy() const
{
	return m_f_SizeX / 2 * static_cast<float>(std::sin(GetRotation()));
}

float DrawObject::GetRotatedOffsetYx() const
{
	return m_f_SizeY / 2 * static_cast<float>(std::sin(GetRotation()));

}
float DrawObject::GetRotatedOffsetYy() const
{
	return m_f_SizeY / 2 * -static_cast<float>(std::cos(GetRotation()));
}


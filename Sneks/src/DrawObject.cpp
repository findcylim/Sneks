#include "DrawObject.h"
#include <cmath>
#include "AEMtx33.h"
#include "AEVec2.h"
#include "AEGraphics.h"

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
	m_f_PositionX = f;
}
void DrawObject::SetPositionY(const float f) {
	m_f_PositionY = f;
}
float DrawObject::GetRotation() const
{
	return m_f_Rotation;
}
float DrawObject::GetVelocity() const
{
	return m_f_Velocity;
}
float DrawObject::GetPositionX() const
{
	return m_f_PositionX;
}
float DrawObject::GetPositionY() const
{
	return m_f_PositionY;
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
	m_f_PositionX = posX;
	m_f_PositionY = posY;
	m_px_Texture = tex;
	m_f_SizeX = sizeX;
	m_f_SizeY = sizeY;
	AEGfxMeshStart();
	AEGfxTriAdd(-(sizeX/2), -(sizeY/2), 0x00FFFFFF, 0, 1,
		sizeX / 2, -(sizeY/2), 0x0000FFFF, 1, 1,
		-(sizeX / 2), sizeY / 2, 0x00FFFF00, 0, 0);

	AEGfxTriAdd(
		sizeX / 2, sizeY / 2, 0x00FFFFFF, 1, 0,
		-(sizeX / 2), sizeY / 2, 0x00FFFFFF, 0, 0,
		sizeX / 2, -(sizeY / 2), 0x00FFFFFF, 1, 1);
	m_px_Obj = AEGfxMeshEnd();
}

DrawObject::~DrawObject(void)
= default;


void DrawObject::Draw() {
	auto* rotMatrix = new AEMtx33();
	AEMtx33Rot(rotMatrix, m_f_Rotation);

	auto* transMatrix = new AEMtx33();
	AEMtx33Trans(transMatrix, m_f_PositionX, m_f_PositionY);

	/*generate global matrix from rot and trans*/
	auto* globalMatrix = new AEMtx33();
	AEMtx33Concat(globalMatrix, transMatrix, rotMatrix);

	//allow transparency to work !!must be first
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1, 1, 1, 1);
	AEGfxTextureSet(m_px_Texture, 0, 0);
	AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	AEGfxSetTransparency(1);
	AEGfxSetPosition(m_f_PositionX, m_f_PositionY);
	AEGfxSetTransform(globalMatrix->m);
	AEGfxMeshDraw(m_px_Obj, AE_GFX_MDM_TRIANGLES);

}


void DrawObject::Update() {
	
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


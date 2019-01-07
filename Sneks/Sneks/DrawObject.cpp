#include "DrawObject.h"
#include "AEMtx33.h"
#include "AEVec2.h"
#include "AEGraphics.h"

void DrawObject::SetTexture(AEGfxTexture* tex) {
	m_pTex = tex;
}
void DrawObject::SetRotation(float f) {
	m_fRotation = f;
}
void DrawObject::SetVelocity(float f) {
	m_fVelocity = f;
}
void DrawObject::SetPositionX(float f) {
	m_fPositionX = f;
}
void DrawObject::SetPositionY(float f) {
	m_fPositionY = f;
}
float DrawObject::GetRotation() {
	return m_fRotation;
}
float DrawObject::GetVelocity() {
	return m_fVelocity;
}
float DrawObject::GetPositionX() {
	return m_fPositionX;
}
float DrawObject::GetPositionY() {
	return m_fPositionY;
}


DrawObject::DrawObject(float posX, float posY, float sizeX, float sizeY, AEGfxTexture* tex)
{
	m_fPositionX = posX;
	m_fPositionY = posY;
	m_pTex = tex;
	m_fSizeX = sizeX;
	m_fSizeY = sizeY;
	AEGfxMeshStart();
	AEGfxTriAdd(-(sizeX/2), -(sizeY/2), 0x00FFFFFF, 0, 1,
		sizeX / 2, -(sizeY/2), 0x0000FFFF, 1, 1,
		-(sizeX / 2), sizeY / 2, 0x00FFFF00, 0, 0);

	AEGfxTriAdd(
		sizeX / 2, sizeY / 2, 0x00FFFFFF, 1, 0,
		-(sizeX / 2), sizeY / 2, 0x00FFFFFF, 0, 0,
		sizeX / 2, -(sizeY / 2), 0x00FFFFFF, 1, 1);
	m_pObj = AEGfxMeshEnd();
}



void DrawObject::Draw() {

	AEMtx33* rotMatrix = new AEMtx33();
	AEMtx33Rot(rotMatrix, m_fRotation);
	AEMtx33* transMatrix = new AEMtx33();
	AEMtx33Trans(transMatrix, m_fPositionX, m_fPositionY);
	/*generate global matrix from rot and trans*/
	AEMtx33* globalMatrix = new AEMtx33();
	AEMtx33Concat(globalMatrix, transMatrix, rotMatrix);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1, 1, 1, 0);
	AEGfxTextureSet(m_pTex, 0, 0);
	AEGfxSetTransparency(1);
	AEGfxSetPosition(m_fPositionX, m_fPositionY);
	AEGfxSetTransform(globalMatrix->m);
	AEGfxMeshDraw(m_pObj, AE_GFX_MDM_TRIANGLES);

}


void DrawObject::Update() {
	
}

float DrawObject::GetRotatedOffsetXX() {
	return m_fSizeX / 2 * (float)cos(GetRotation()) ;

}
float DrawObject::GetRotatedOffsetXY() {
	return m_fSizeX / 2 * (float)sin(GetRotation());
}

float DrawObject::GetRotatedOffsetYX() {
	return m_fSizeY / 2 * (float)sin(GetRotation());

}
float DrawObject::GetRotatedOffsetYY() {
	return m_fSizeY / 2 * -(float)cos(GetRotation());
}
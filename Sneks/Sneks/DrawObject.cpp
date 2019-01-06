#include "DrawObject.h"
#include "AEMtx33.h"
#include "AEVec2.h"
#define SCREENSIZE_X 1500
#define SCREENSIZE_Y 900
#define MAXVELOCITY 5.0f
#define ACCEL_FAC 0.05f
#define BRAKE_FAC 0.05f
#define TURN_SPEED 0.02f
#define FRICTION 0.02f

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


	/*this shouldnt be here, only for testing*/
	if (GetAsyncKeyState(VK_UP)) {
		m_fVelocity -= ACCEL_FAC;
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		m_fVelocity += BRAKE_FAC;
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		m_fRotation += TURN_SPEED;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		m_fRotation -= TURN_SPEED;
	}
	/*end removal*/

	//limit max velocity
	if (m_fVelocity >= MAXVELOCITY)
		m_fVelocity = MAXVELOCITY;
	else if (m_fVelocity <= -MAXVELOCITY)
		m_fVelocity = -MAXVELOCITY;

	//if out of screen, pop to the other side of screen X
	if (m_fPositionX > SCREENSIZE_X / 2 + m_fSizeX / 2)
		m_fPositionX = -SCREENSIZE_X / 2 - m_fSizeX / 2;
	else if (m_fPositionX < -SCREENSIZE_X / 2 - m_fSizeX / 2)
		m_fPositionX = SCREENSIZE_X / 2 + m_fSizeX / 2;
	
	//if out of screen, pop to the other side of screen Y
	if (m_fPositionY > SCREENSIZE_Y / 2 + m_fSizeY / 2)
		m_fPositionY = -SCREENSIZE_Y / 2 - m_fSizeY / 2;
	else if (m_fPositionY < -SCREENSIZE_Y / 2 - m_fSizeY / 2)
		m_fPositionY = SCREENSIZE_Y / 2 + m_fSizeY / 2;


	/*clamp low velocity to 0 so its not jittery*/
	if (m_fVelocity >= -0.05 && m_fVelocity <= 0.05)
		m_fVelocity = 0;
	else if (m_fVelocity < 0)
		m_fVelocity += FRICTION;
	else if (m_fVelocity > 0)
		m_fVelocity -= FRICTION;


	//apply the velocity
	AEVec2 testPos;
	AEVec2FromAngle(&testPos, m_fRotation);

	m_fPositionX += testPos.x * m_fVelocity;
	m_fPositionY += testPos.y * m_fVelocity;

}

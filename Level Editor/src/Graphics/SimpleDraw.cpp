#include "SimpleDraw.h"
#include <cmath>
#include "AEMtx33.h"
#include "AEVec2.h"
#include "AEGraphics.h"


HTColor SimpleDraw::GetColor() const
{
	return m_f_RgbaColor;
}

HTVector2 SimpleDraw::GetScale() const
{
	return m_x_Scale;
}

void SimpleDraw::SetScale(float x,float y)
{
	m_x_Scale.x = x;
	m_x_Scale.y = y;
}
void SimpleDraw::SetColor(float red, float green,float blue, float alpha)
{
	m_f_RgbaColor.red   = red;
	m_f_RgbaColor.green = green;
	m_f_RgbaColor.blue  = blue;
	m_f_RgbaColor.alpha = alpha;
}

void SimpleDraw::SetAlpha(float alpha)
{
	m_f_RgbaColor.alpha = alpha;
}

void SimpleDraw::SetRotation(const float f) {
	m_f_Rotation = f;
}
void SimpleDraw::SetVelocity(const float velocity) {
	m_f_Velocity = velocity;
	//m_f_Velocity.y = velocityY;
}
void SimpleDraw::SetPosition(const float positionX, const float positionY) {
	m_x_Position.x = positionX;
	m_x_Position.y = positionY;
}
//void SimpleDraw::SetPositionX(const float f) {
//	m_x_Position.x = f;
//}
//void SimpleDraw::SetPositionY(const float f) {
//	m_x_Position.y = f;
//}

HTVector2 SimpleDraw::GetMin() const
{
	HTVector2 min = {};
	min.x = m_x_Position.x - m_x_Size.x * m_x_Scale.x / 2;
	min.y = m_x_Position.y - m_x_Size.y * m_x_Scale.y / 2;
	return min;
}

HTVector2 SimpleDraw::GetMax() const
{
	HTVector2 max = {};
	max.x = m_x_Position.x + m_x_Size.x * m_x_Scale.x / 2;
	max.y = m_x_Position.y + m_x_Size.y * m_x_Scale.y / 2;
	return max;
}

float SimpleDraw::GetRotation() const
{
	return m_f_Rotation;
}
float SimpleDraw::GetVelocity() const
{
	return m_f_Velocity;
}
HTVector2 SimpleDraw::GetPosition() const
{
	return m_x_Position;
}


float SimpleDraw::GetSizeX() const
{
	return m_x_Size.x;
}

float SimpleDraw::GetSizeY() const
{
	return m_x_Size.y;
}

const char* SimpleDraw::GetName() const
{
	return m_c_Name;
}

void SimpleDraw::SetPositionX(const float f) {
	m_x_Position.x = f;
}
void SimpleDraw::SetPositionY(const float f) {
	m_x_Position.y = f;
}

SimpleDraw::SimpleDraw(float posX, float posY, float sizeX, float sizeY, const char * name)
{
	m_x_Position = { posX,posY };
	m_x_Size.x = sizeX;
	m_x_Size.y = sizeY;


	size_t nameLen = strlen(name) + 1;
	m_c_Name = new char[nameLen];
	strcpy_s(m_c_Name, nameLen, name);

	AEGfxMeshStart();
	AEGfxTriAdd(-(sizeX / 2), -(sizeY / 2), 0x00FFFFFF, 0, 1,
		sizeX / 2, -(sizeY / 2), 0x0000FFFF, 1, 1,
		-(sizeX / 2), sizeY / 2, 0x00FFFF00, 0, 0);

	AEGfxTriAdd(
		sizeX / 2, sizeY / 2, 0x00FFFFFF, 1, 0,
		-(sizeX / 2), sizeY / 2, 0x00FFFFFF, 0, 0,
		sizeX / 2, -(sizeY / 2), 0x00FFFFFF, 1, 1);
	m_px_Obj = AEGfxMeshEnd();

	m_po_GlobalMatrix = new AEMtx33();
	m_po_RotationMatrix = new AEMtx33();
	m_po_TranslationMatrix = new AEMtx33();

}

SimpleDraw::SimpleDraw(const SimpleDraw& drawObj)
{
	SetPosition(drawObj.m_x_Position.x, drawObj.m_x_Position.y);
	m_f_Rotation = drawObj.GetRotation();
	m_x_Size = { drawObj.GetSizeX(),drawObj.GetSizeY() };

	size_t nameLen = strlen(drawObj.m_c_Name) + 1;
	m_c_Name = new char[nameLen];
	strcpy_s(m_c_Name, nameLen, drawObj.m_c_Name);


	AEGfxMeshStart();

	m_px_Obj = drawObj.m_px_Obj;

	m_po_GlobalMatrix = new AEMtx33();
	m_po_RotationMatrix = new AEMtx33();
	m_po_TranslationMatrix = new AEMtx33();
}

SimpleDraw::~SimpleDraw(void)
{
	delete m_po_GlobalMatrix;
	delete m_po_TranslationMatrix;
	delete m_po_RotationMatrix;

}

void SimpleDraw::Draw() 
{
	AEMtx33ScaleApply(m_po_RotationMatrix, m_po_RotationMatrix, m_x_Scale.x, m_x_Scale.y);
	AEMtx33Rot(m_po_RotationMatrix, m_f_Rotation);
	AEMtx33Trans(m_po_TranslationMatrix, m_x_Position.x, m_x_Position.y);
	/*generate global matrix from rot and trans*/
	AEMtx33Concat(m_po_GlobalMatrix, m_po_TranslationMatrix, m_po_RotationMatrix);

	AEMtx33TransApply(m_po_GlobalMatrix, m_po_GlobalMatrix, m_f_GlobalCameraOffsetX, m_f_GlobalCameraOffsetY);

	AEMtx33ScaleApply(m_po_GlobalMatrix, m_po_GlobalMatrix, m_f_GlobalScale, m_f_GlobalScale);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	AEGfxSetTintColor(m_f_RgbaColor.red, m_f_RgbaColor.green, m_f_RgbaColor.blue, m_f_RgbaColor.alpha);
	AEGfxSetTransparency(m_f_RgbaColor.alpha);
	AEGfxSetPosition(m_x_Position.x, m_x_Position.y);
	AEGfxSetTransform(m_po_GlobalMatrix->m);
	AEGfxMeshDraw(m_px_Obj, AE_GFX_MDM_TRIANGLES);
}

float SimpleDraw::GetRotatedOffsetXx() const
{
	return m_x_Size.x / 2 * static_cast<float>(std::cos(GetRotation())) ;
}
float SimpleDraw::GetRotatedOffsetXy() const
{
	return m_x_Size.x / 2 * static_cast<float>(std::sin(GetRotation()));
}

float SimpleDraw::GetRotatedOffsetYx() const
{
	return m_x_Size.y / 2 * static_cast<float>(std::sin(GetRotation()));

}
float SimpleDraw::GetRotatedOffsetYy() const
{
	return m_x_Size.y / 2 * -static_cast<float>(std::cos(GetRotation()));
}

HTVector2 SimpleDraw::ApplyVelocity(float dt)
{
	auto forwardVelocity = GetForwardVelocity();
	m_x_Position.x += forwardVelocity.x * dt;
	m_x_Position.y += forwardVelocity.y * dt;
	return forwardVelocity;
}

HTVector2 SimpleDraw::GetForwardVelocity()
{
	//apply the velocity
	AEVec2 forwardVector;
	AEVec2FromAngle(&forwardVector, m_f_Rotation);
	HTVector2 forwardVelocity ={ forwardVector.x * m_f_Velocity, forwardVector.y * m_f_Velocity };
	return forwardVelocity;
}
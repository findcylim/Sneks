#include "DrawObject.h"
#include <cmath>
#include "AEMtx33.h"
#include "AEVec2.h"
#include "AEGraphics.h"


AEGfxVertexList* DrawObject::GetMesh() const
{
	return m_px_Obj;
}

HTColor DrawObject::GetColor() const
{
	return m_f_RgbaColor;
}

float DrawObject::GetScale() const
{
	return m_f_Scale;
}

void DrawObject::SetScale(float f)
{
	m_f_Scale = f;
}
void DrawObject::SetColor(float red, float green,float blue, float alpha)
{
	m_f_RgbaColor.red   = red;
	m_f_RgbaColor.green = green;
	m_f_RgbaColor.blue  = blue;
	m_f_RgbaColor.alpha = alpha;
}

void DrawObject::SetAlpha(float alpha)
{
	m_f_RgbaColor.alpha = alpha;
}

void DrawObject::SetTexture(AEGfxTexture* tex) {
	m_px_Texture = tex;
}
void DrawObject::SetRotation(const float f) {
	m_f_Rotation = f;
}
void DrawObject::SetVelocity(const float velocity) {
	m_f_Velocity = velocity;
	//m_f_Velocity.y = velocityY;
}
void DrawObject::SetPosition(const float positionX, const float positionY) {
	m_x_Position.x = positionX;
	m_x_Position.y = positionY;
}
//void DrawObject::SetPositionX(const float f) {
//	m_x_Position.x = f;
//}
//void DrawObject::SetPositionY(const float f) {
//	m_x_Position.y = f;
//}

HTVector2 DrawObject::GetMin() const
{
	HTVector2 min = {};
	min.x = m_x_Position.x - m_x_Size.x * m_f_Scale / 2;
	min.y = m_x_Position.y - m_x_Size.y * m_f_Scale / 2;
	return min;
}

HTVector2 DrawObject::GetMax() const
{
	HTVector2 max = {};
	max.x = m_x_Position.x + m_x_Size.x * m_f_Scale / 2;
	max.y = m_x_Position.y + m_x_Size.y * m_f_Scale / 2;
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
HTVector2 DrawObject::GetPosition() const
{
	return m_x_Position;
}


float DrawObject::GetSizeX() const
{
	return m_x_Size.x;
}

float DrawObject::GetSizeY() const
{
	return m_x_Size.y;
}


DrawObject::DrawObject(const float posX, const float posY, const float sizeX, const float sizeY, AEGfxTexture* tex)
{
	m_x_Position={ posX,posY };
	m_px_Texture  = tex;
	m_x_Size.x     = sizeX;
	m_x_Size.y     = sizeY;
	AEGfxMeshStart();	AEGfxTriAdd(-(sizeX/2), -(sizeY/2), 0x00FFFFFF, 0, 1,
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

DrawObject::DrawObject(const float posX, const float posY, const float sizeX, const float sizeY, AEGfxTexture* tex, AEGfxVertexList* mesh)
{
	m_x_Position={ posX,posY };
	m_px_Texture  = tex;
	m_x_Size.x     = sizeX;
	m_x_Size.y     = sizeY;
	m_px_Obj = mesh;
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

	AEMtx33TransApply(m_po_GlobalMatrix, m_po_GlobalMatrix, m_f_GlobalCameraOffsetX, m_f_GlobalCameraOffsetY);

	AEMtx33ScaleApply(m_po_GlobalMatrix, m_po_GlobalMatrix, m_f_GlobalScale, m_f_GlobalScale);

	//allow transparency to work !! must be first
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetTintColor(m_f_RgbaColor.red, m_f_RgbaColor.green, m_f_RgbaColor.blue, m_f_RgbaColor.alpha);
	AEGfxTextureSet(m_px_Texture, 0, 0);
	AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	AEGfxSetTransparency(1);
	AEGfxSetPosition(m_x_Position.x, m_x_Position.y);
	AEGfxSetTransform(m_po_GlobalMatrix->m);
	AEGfxMeshDraw(m_px_Obj, AE_GFX_MDM_TRIANGLES);
}

float DrawObject::GetRotatedOffsetXx() const
{
	return m_x_Size.x / 2 * static_cast<float>(std::cos(GetRotation())) ;
}
float DrawObject::GetRotatedOffsetXy() const
{
	return m_x_Size.x / 2 * static_cast<float>(std::sin(GetRotation()));
}

float DrawObject::GetRotatedOffsetYx() const
{
	return m_x_Size.y / 2 * static_cast<float>(std::sin(GetRotation()));

}
float DrawObject::GetRotatedOffsetYy() const
{
	return m_x_Size.y / 2 * -static_cast<float>(std::cos(GetRotation()));
}

HTVector2 DrawObject::ApplyVelocity(float dt)
{
	auto forwardVelocity = GetForwardVelocity();
	m_x_Position.x += forwardVelocity.x * dt;
	m_x_Position.y += forwardVelocity.y * dt;
	return forwardVelocity;
}

HTVector2 DrawObject::GetForwardVelocity()
{
	//apply the velocity
	AEVec2 forwardVector;
	AEVec2FromAngle(&forwardVector, m_f_Rotation);
	HTVector2 forwardVelocity ={ forwardVector.x * m_f_Velocity, forwardVector.y * m_f_Velocity };
	return forwardVelocity;
}
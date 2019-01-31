#include "DrawComponent.h"


DrawComponent::DrawComponent(BaseComponent* transformComponent) :
	m_po_TransformComponent{ static_cast<TransformComponent*>(transformComponent) }
{
	assert(m_po_TransformComponent != nullptr);
	assert(m_po_TransformComponent->m_x_ComponentID == kComponentTransform);
}

AEGfxVertexList* DrawComponent::GetMesh() const
{
	return m_px_Obj;
}

HTColor DrawComponent::GetColor() const
{
	return m_f_RgbaColor;
}

void DrawComponent::SetColor(float red, float green, float blue, float alpha)
{
	m_f_RgbaColor.red = red;
	m_f_RgbaColor.green = green;
	m_f_RgbaColor.blue = blue;
	m_f_RgbaColor.alpha = alpha;
}

void DrawComponent::SetAlpha(float alpha)
{
	m_f_RgbaColor.alpha = alpha;
}

void DrawComponent::SetTexture(AEGfxTexture* tex) 
{
	m_px_Texture = tex;
}

float DrawComponent::GetSizeX() const
{
	return m_x_Size.x;
}

float DrawComponent::GetSizeY() const
{
	return m_x_Size.y;
}

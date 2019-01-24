#ifndef BACKGROUND_H
#define BACKGROUND_H
#pragma once
#include "DrawObject.h"
#include <vector>
class Background
{
private:
	std::vector<DrawObject*> m_BgInstances ={};

public:
	Background(int instancesX, int instancesY, AEGfxTexture* bgTexture);
	Background() = delete;
	~Background();
	void Draw();
};

#endif // BACKGROUND_H
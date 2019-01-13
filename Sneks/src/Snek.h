#ifndef SNEK_H
#define SNEK_H
#pragma once
#include "SnekHead.h"
#include "SnekBody.h"
#include <vector>

class Snek
{
private:
	std::vector<SnekBody*> m_v_BodyParts;
	SnekHead*              m_po_Head;
public:
	Snek(SnekHead*);
	~Snek();
	void AddBodyPart(SnekBody*);
	void Update();
	void Draw();
};
#endif //SNAKE_H
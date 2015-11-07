#pragma once
#include "GameObject.h"

class DrawableGameObject : public GameObject
{
public:
	virtual void Desenhar() = 0;

};


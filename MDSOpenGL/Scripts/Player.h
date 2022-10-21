#pragma once
#include "GameObject.h"

class CPlayer : public CGameObject
{
public:
	CPlayer();
	void Update() override;
};
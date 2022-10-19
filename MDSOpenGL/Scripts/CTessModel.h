#pragma once
#include "GameObject.h"

class CTessModel : public CGameObject
{
protected:
	bool m_bUpdateVertexArray;

	CTessModel();
	virtual void Draw() override;
};


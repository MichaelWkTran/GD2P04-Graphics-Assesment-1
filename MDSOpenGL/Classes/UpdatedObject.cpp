#include "UpdatedObject.h"
#include "GameManager.h"

CUpdatedObject::CUpdatedObject()
{
	m_bStartCalled = false;
	m_bDeleteUpdatedObject = false;
	GetGameManager().m_dequeUpdatedObject.emplace_back(this);
}
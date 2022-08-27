#pragma once
#include <deque>
#include "UpdatedObject.h"

/*
CGameManager managages the game rules
*/
class CGameManager
{
	friend CUpdatedObject;

private:
	static CGameManager* m_pSingleton;

protected:
	std::deque<CUpdatedObject*> m_dequeUpdatedObject;
	
	~CGameManager();

public:
	//Methods
	CGameManager();
	CGameManager(const CGameManager&) = delete;
	CGameManager& operator= (const CGameManager&) = delete;
	
	void DestroyImmediate(CUpdatedObject* _pUpdatedObject);
	void DestroyImmediate(CUpdatedObject*& _pUpdatedObject);
	void Clear();

	virtual void Update();

	//Get Set Methods
	static CGameManager& GetSingleton();
};

#define GetGameManager CGameManager::GetSingleton
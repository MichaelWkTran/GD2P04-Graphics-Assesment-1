#pragma once
#include <deque>
#include "UpdatedObject.h"

/*
CGameManager managages the game rules
*/
class CGameManager
{
private:
	static CGameManager* m_pSingleton;

protected:
	~CGameManager();
	std::deque<CUpdatedObject*> m_dequeUpdatedObject;

public:
	static CGameManager& GetSingleton();

	CGameManager();
	CGameManager(const CGameManager&) = delete;
	CGameManager& operator= (const CGameManager&) = delete;
	
	template <class T>
	T* CreateObject();
	void Clear();

	virtual void Update();
};

template<class T>
inline T* CGameManager::CreateObject()
{
	m_dequeUpdatedObject.emplace_back(new T);
	m_dequeUpdatedObject.back()->m_pGameManager = this;
	return (T*)m_dequeUpdatedObject.back();
}

#define GetGameManager CGameManager::GetSingleton
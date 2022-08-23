#include "GameManager.h"
#include "Camera.h"
#include "../Main.h"
#include <iostream>

CGameManager* CGameManager::m_pSingleton = nullptr;

CGameManager::CGameManager()
{
	if (m_pSingleton == nullptr) { m_pSingleton = this; }
	else if (m_pSingleton != this) delete this;
}

CGameManager& CGameManager::GetSingleton()
{
	if (m_pSingleton == nullptr)
	{
		std::cout << "ERROR: The Game manager class must be created before requesting it. A default game manager is created instead";
		m_pSingleton = new CGameManager();
	}

	return *m_pSingleton;
}

CGameManager::~CGameManager()
{
	Clear();
}

void CGameManager::Clear()
{
	const int iGameObjectsCount = m_dequeUpdatedObject.size();
	for (int i = 0; i < iGameObjectsCount; i++)
	{
		delete m_dequeUpdatedObject.front();
		m_dequeUpdatedObject.pop_front();
	}
}

void CGameManager::Update()
{
	for (auto& pGameObject : m_dequeUpdatedObject) pGameObject->Start();
	for (auto& pGameObject : m_dequeUpdatedObject) pGameObject->BeginUpdate();
	for (auto& pGameObject : m_dequeUpdatedObject) pGameObject->Update();
	for (auto& pGameObject : m_dequeUpdatedObject) pGameObject->EndUpdate();
	for (auto& pGameObject : m_dequeUpdatedObject) pGameObject->Draw();

	//Delete GameObjects
	for (int i = 0; i < (int)m_dequeUpdatedObject.size(); i++)
	{
		if (!m_dequeUpdatedObject[i]->GetDeleteUpdatedObject()) continue;

		CUpdatedObject* pDeletedGameObject = m_dequeUpdatedObject[i];
		m_dequeUpdatedObject.erase(m_dequeUpdatedObject.begin() + i);
		delete pDeletedGameObject;
	}
}
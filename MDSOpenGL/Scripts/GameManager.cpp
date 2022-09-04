/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: GameManaeger.cpp																								*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#include "GameManager.h"
#include "Camera.h"
#include "Main.h"
#include <iostream>

CGameManager* CGameManager::m_pSingleton = nullptr;

CGameManager::CGameManager()
{
	//Ensure that the game manager is a singleton class
	if (m_pSingleton == nullptr) { m_pSingleton = this; }
	else if (m_pSingleton != this) delete this;
}

CGameManager& CGameManager::GetSingleton()
{
	//Create a game manager if none is created yet
	if (m_pSingleton == nullptr)
	{
		std::cout << "ERROR: The Game manager class must be created before requesting it. A default game manager is created instead";
		m_pSingleton = new CGameManager();
	}

	//Get the game manager
	return *m_pSingleton;
}

CGameManager::~CGameManager()
{
	Clear();
}

/*DestroyImmediate is used for destroying the updated object imediately outside of the update loop. */
void CGameManager::DestroyImmediate(CUpdatedObject* _pUpdatedObject)
{
	//Check whether the given pointer is valid
	if (_pUpdatedObject == nullptr)
	{
		std::cout << "ERROR: Can not use DestroyImmediate on nullptr";
		return;
	}

	//Find the requested object and delete it
	for (int i = 0; i < (int)m_dequeUpdatedObject.size(); i++)
	{
		if (m_dequeUpdatedObject[i] != _pUpdatedObject) continue;

		m_dequeUpdatedObject.erase(m_dequeUpdatedObject.begin() + i);
		delete _pUpdatedObject;

		return;
	}
}

/*DestroyImmediate is used for destroying the updated object imediately outside of the update loop. */
void CGameManager::DestroyImmediate(CUpdatedObject*& _pUpdatedObject)
{
	//Check whether the given pointer is valid
	if (_pUpdatedObject == nullptr)
	{
		std::cout << "ERROR: Can not use DestroyImmediate on nullptr";
		return;
	}

	//Find the requested object and delete it
	for (int i = 0; i < (int)m_dequeUpdatedObject.size(); i++)
	{
		if (m_dequeUpdatedObject[i] != _pUpdatedObject) continue;

		m_dequeUpdatedObject.erase(m_dequeUpdatedObject.begin() + i);
		delete _pUpdatedObject;

		//Ensure that the given pointer is nullptr
		_pUpdatedObject = nullptr;

		return;
	}
}

/*Delete all updated objects*/
void CGameManager::Clear()
{
	const int iGameObjectsCount = m_dequeUpdatedObject.size();
	for (int i = 0; i < iGameObjectsCount; i++)
	{
		delete m_dequeUpdatedObject.front();
		m_dequeUpdatedObject.pop_front();
	}
}

/*Called every game frame and updates all game objects*/
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
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

//------------------------------------------------------------------------------------------------------------------------
// Procedure: CGameManager()
//	 Purpose: Ensure that the game manager is a singeton

CGameManager::CGameManager()
{
	//Ensure that the game manager is a singleton class
	if (m_pSingleton == nullptr) { m_pSingleton = this; }
	else if (m_pSingleton != this) delete this;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetSingleton()
//	 Purpose: Gets the current game manager, if none exists create one
//	 Returns: The current game manager

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

//------------------------------------------------------------------------------------------------------------------------
// Procedure: ~CGameManager()
//	 Purpose: Delete all stored game objects when the game manager is destroyed

CGameManager::~CGameManager()
{
	Clear();
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: DestroyImmediate()
//	 Purpose: DestroyImmediate is used for destroying the given updated object, _pUpdatedObject, imediately outside of the update loop.

const std::deque<CUpdatedObject*>& CGameManager::GetUpdatedObjects() const
{
	return m_dequeUpdatedObject;
}

void CGameManager::DestroyImmediate(CUpdatedObject*&& _pUpdatedObject)
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

//------------------------------------------------------------------------------------------------------------------------
// Procedure: DestroyImmediate()
//	 Purpose: DestroyImmediate is used for destroying the given updated object, _pUpdatedObject, imediately outside of the update loop.

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

void CGameManager::DestroyImmediate(unsigned int _uiIndex)
{
	CUpdatedObject* pUpdatedObject = m_dequeUpdatedObject[_uiIndex];

	m_dequeUpdatedObject.erase(m_dequeUpdatedObject.begin() + _uiIndex);
	delete pUpdatedObject;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: Clear()
//	 Purpose: Delete all updated objects

void CGameManager::Clear()
{
	const int iGameObjectsCount = m_dequeUpdatedObject.size();
	for (int i = 0; i < iGameObjectsCount; i++)
	{
		delete m_dequeUpdatedObject.front();
		m_dequeUpdatedObject.pop_front();
	}
}

void CGameManager::UpdateObjectsInWorld()
{
	for (auto& pGameObject : m_dequeUpdatedObject) pGameObject->Start();
	for (auto& pGameObject : m_dequeUpdatedObject) pGameObject->BeginUpdate();
	for (auto& pGameObject : m_dequeUpdatedObject) pGameObject->Update();
	for (auto& pGameObject : m_dequeUpdatedObject) pGameObject->EndUpdate();
}

void CGameManager::DrawObjectsInWorld()
{
	for (auto& pGameObject : m_dequeUpdatedObject) pGameObject->Draw();
}

void CGameManager::DeleteObjectsInWorld()
{
	//Search all objects in the world
	for (int i = 0; i < (int)m_dequeUpdatedObject.size(); i++)
	{
		//Skip objects not maked for deletion
		if (!m_dequeUpdatedObject[i]->GetDeleteUpdatedObject()) continue;

		//Delete object
		DestroyImmediate(i);
	}
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: Update()
//	 Purpose: Called every game frame and updates all game objects

void CGameManager::Update()
{
	UpdateObjectsInWorld();
	DrawObjectsInWorld();
	DeleteObjectsInWorld();
}
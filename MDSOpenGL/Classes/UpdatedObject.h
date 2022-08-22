#pragma once
class CGameManager;

class CUpdatedObject
{
private:
	bool m_bStartCalled;
	bool m_bDeleteUpdatedObject;

public:
	CGameManager* m_pGameManager = nullptr;
	
	CUpdatedObject()
	{
		m_bStartCalled = false;
		m_bDeleteUpdatedObject = false;
		m_pGameManager = nullptr;
	}
	~CUpdatedObject() {}

	const bool GetDeleteUpdatedObject() const
	{
		return m_bDeleteUpdatedObject;
	}
	void DeleteUpdatedObject()
	{
		m_bDeleteUpdatedObject = true;
	}

	virtual void Start()
	{
		if (m_bStartCalled) return;
		m_bStartCalled = true;
	};
	virtual void BeginUpdate() {}
	virtual void Update() {}
	virtual void EndUpdate() {}
	virtual void Draw() {};
};
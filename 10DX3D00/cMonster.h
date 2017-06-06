#pragma once

class cSkinnedMesh;

class cMonster
{
public:
	cMonster();
	virtual ~cMonster();

protected:
	enum eMonsterState
	{
		E_IDLE,
		E_ATTACK,
		E_FIND_PATH,
		E_DEAD,
		E_STUN,
	};

	cSkinnedMesh*		m_pSkinnedMesh;
	eMonsterState		m_eMonsterState;

	HANDLE				m_hPathThread;

	bool				m_isDie;

public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
	virtual void StartThread();
	virtual void FindPath();
};


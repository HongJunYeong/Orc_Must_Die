#pragma once

class cSkinnedMesh;
class cAStar;

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

	//astar
	cAStar*				m_pAStar;
	ST_TILE_INFO		m_stEndTile;
	vector<D3DXVECTOR3>	m_vecDest;
	int					m_nPrevStart;

	D3DXVECTOR3			m_vPosition;
	D3DXVECTOR3			m_vDirection;

	HANDLE				m_hPathThread;

	float				m_fRotY;
	bool				m_isDie;
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
	virtual void Release();
	virtual void StartThread();
	virtual void FindPath();
};


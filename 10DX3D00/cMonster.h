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
		E_NONE,
		E_DEAD,
		E_STUN,
	};

	cSkinnedMesh*		m_pSkinnedMesh;
	eMonsterState		m_eMonsterState;

	//astar
	cAStar*				m_pAStar;
	ST_TILE_INFO		m_stEndTile;
	vector<D3DXVECTOR3>	m_vecDest;
	//D3DXVECTOR3			m_vFinalDest;
	ST_TILE_INFO		m_stFinalDestTile;
	int					m_nPrevStart;

	D3DXVECTOR3			m_vPosition;
	D3DXVECTOR3			m_vDirection;
	D3DXVECTOR3			m_vScale;
	D3DXMATRIXA16		m_matWorld;

	HANDLE				m_hPathThread;
	HANDLE				m_hMutex;

	ST_SPHERE			m_stTraceSphere;   //탐지 범위
	ST_SPHERE			m_stAttackSphere;  //공격 범위
	ST_SPHERE			m_stLWeaponSphere; //왼쪽 무기 범위
	ST_SPHERE			m_stRWeaponSphere; //오른쪽 무기 범위
	ST_SPHERE			m_stHitSphere;     //피격 범위

	LPD3DXMESH          m_pAttackSphere;   //공격 범위 표시
	LPD3DXMESH          m_pTraceSphere;      //탐지 범위 표시
	LPD3DXMESH		    m_pLWeaponSphere;   //왼쪽 무기 범위 표시
	LPD3DXMESH			m_pRWeaponSphere;   //오른쪽 무기 범위 표시
	LPD3DXMESH			m_pHitSphere;      //피격 범위 표시

	int					m_nAnimIndex; //애니메이션 위치
	bool				m_isCollision; //충돌 확인

	float				m_fRotY;
	bool				m_isDie;
	bool				m_isFindPath;
	bool				m_isGameStart;

	bool				m_isTrace;
	bool				m_isTraceForFinalDest;
	bool				m_isAttack;
	float				m_fHP;

	int					m_nStartTime;
	int					m_nEndTime;
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
	virtual void Release();
	virtual void StartThread();
	virtual void FindPath();
	virtual void ThreadResume();
	virtual void StopThread();

	virtual void Sphere_Render();
	virtual void RWeaponSphere_Render(string name);
	virtual void LWeaponSphere_Render(string name);
	virtual float Distance_Between_Three_Points(D3DXVECTOR3 v1, D3DXVECTOR3 v2);
	virtual void Move();

	virtual void CheckTraceTime();

	virtual void TraceSeach();
	virtual bool AttackSearch();
};


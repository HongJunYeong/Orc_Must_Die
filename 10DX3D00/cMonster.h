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

	ST_SPHERE			m_stTraceSphere;   //Ž�� ����
	ST_SPHERE			m_stAttackSphere;  //���� ����
	ST_SPHERE			m_stLWeaponSphere; //���� ���� ����
	ST_SPHERE			m_stRWeaponSphere; //������ ���� ����
	ST_SPHERE			m_stHitSphere;     //�ǰ� ����

	LPD3DXMESH          m_pAttackSphere;   //���� ���� ǥ��
	LPD3DXMESH          m_pTraceSphere;      //Ž�� ���� ǥ��
	LPD3DXMESH		    m_pLWeaponSphere;   //���� ���� ���� ǥ��
	LPD3DXMESH			m_pRWeaponSphere;   //������ ���� ���� ǥ��
	LPD3DXMESH			m_pHitSphere;      //�ǰ� ���� ǥ��

	int					m_nAnimIndex; //�ִϸ��̼� ��ġ
	bool				m_isCollision; //�浹 Ȯ��

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

	virtual int FindArrForXZ(float x, float z);
	virtual int FindArr(int idX, int idY);

	virtual void TraceSeach();
	virtual bool AttackSearch();
};


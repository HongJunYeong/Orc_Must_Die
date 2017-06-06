#include "stdafx.h"
#include "cMonster.h"

unsigned int __stdcall FindPathThread(LPVOID p);

cMonster::cMonster()
{
}


cMonster::~cMonster()
{
}

void cMonster::Setup()
{
	m_pSkinnedMesh = NULL;
	m_hPathThread = NULL;
	m_eMonsterState = E_IDLE;

	m_vPosition = D3DXVECTOR3(0, 0, 0);
	m_vPosition = D3DXVECTOR3(0, 0, 1.0f);
	
	m_fRotY = 0.0f;
	m_isDie = false;
}

void cMonster::Update()
{
}

void cMonster::Render()
{
}

void cMonster::StartThread()
{
	m_hPathThread = (HANDLE)_beginthreadex(NULL, 0, FindPathThread, this, 0, NULL);
}

void cMonster::FindPath()
{
	while (true)
	{
		if (m_isDie) break;


	}
}

unsigned int __stdcall FindPathThread(LPVOID p)
{
	cMonster* g = (cMonster*)p;
	g->FindPath();

	return 0;
}

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

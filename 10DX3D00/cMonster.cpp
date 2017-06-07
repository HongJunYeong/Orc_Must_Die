#include "stdafx.h"
#include "cMonster.h"

#include "cSkinnedMesh.h"
#include "cAStar.h"
#include "cTile.h"

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
	m_pAStar = new cAStar;
	m_pAStar->Setup();
	m_eMonsterState = E_IDLE;

	m_vPosition = D3DXVECTOR3(0, 0, 0);
	m_vPosition = D3DXVECTOR3(0, 0, 1.0f);
	
	m_fRotY = 0.0f;
	m_isDie = false;

	m_stEndTile = g_pGameManager->GetStageOneTile()->FindTile(ST_TILE_INFO::DEST);
}

void cMonster::Update()
{
}

void cMonster::Render()
{
}

void cMonster::Release()
{
	if (TerminateThread(m_hPathThread, 0))
	{
		CloseHandle(m_hPathThread);
	}

	SAFE_DELETE(m_pSkinnedMesh);
}

void cMonster::StartThread()
{
	m_hPathThread = (HANDLE)_beginthreadex(NULL, 0, FindPathThread, this, 0, NULL);
}

void cMonster::FindPath()
{
	int FindPathStartTime = GetTickCount();
	int FindPathEndTime = GetTickCount() + 1000;

	while (true)
	{
		if (m_isDie) break;

		FindPathStartTime++;

		if (FindPathStartTime >= FindPathEndTime)
		{
			FindPathEndTime = FindPathStartTime + 1000;

			vector<ST_TILE_INFO> vecTile
				= g_pGameManager->GetStageOneTile()->GetTileInfoValue();

			int start = g_pGameManager->GetStageOneTile()->FindArr(m_vPosition.x, m_vPosition.y);

			g_pGameManager->GetStageOneTile()->GetTileInfo()[m_nPrevStart].type = ST_TILE_INFO::NONE;
			g_pGameManager->GetStageOneTile()->GetTileInfo()[start].type = ST_TILE_INFO::MONSTER;

			m_nPrevStart = start;

			vecTile[start].aStarType = ST_TILE_INFO::START;
			for each(ST_TILE_INFO tile in vecTile)
			{
				if ((tile.idX == m_stEndTile.idX)
					&& (tile.idY == m_stEndTile.idY))
				{
					tile.aStarType = ST_TILE_INFO::END;
					break;
				}
			}

			m_vecDest = m_pAStar->FindPath(&vecTile[start], &m_stEndTile, vecTile);
		}
	}
}

unsigned int __stdcall FindPathThread(LPVOID p)
{
	cMonster* g = (cMonster*)p;
	g->FindPath();

	return 0;
}

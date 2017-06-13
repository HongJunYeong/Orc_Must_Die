#include "stdafx.h"
#include "cGameManager.h"

#include "cMonster.h"
#include "cCharacter.h"
#include "cTile.h"

#include "cFelorcAxe.h"
#include "cFelorcDire.h"
#include "cFelorcSword.h"
#include "cFelorcWarriorBoss.h"
#include "cFelorcWarriorSword.h"

cGameManager::cGameManager()
	:m_sPlayerName("")
	,m_pStageOneTile(NULL)
{
	m_hSem = CreateSemaphore(NULL, 4, 4, NULL);
}


cGameManager::~cGameManager()
{
}

void cGameManager::Destroy()
{
	SAFE_DELETE(m_pStageOneTile);

	for each(auto p in m_vecMonster)
		p->Release();

	for each(auto p in m_vecCharacter)
		p->Release();

	m_vecMonster.clear();
	CloseHandle(m_hSem);
}

void cGameManager::SetPlayerName(string name)
{
	m_sPlayerName = name;
}

string cGameManager::GetPlayerName()
{
	return m_sPlayerName;
}

cTile * cGameManager::GetStageOneTile()
{
	return m_pStageOneTile;
}

void cGameManager::StageOneTileSetup()
{
	m_pStageOneTile = new cTile;

	m_pStageOneTile->Setup(300, 1.0f);
}

void cGameManager::StageOneSetup()
{
	for (int i = 0; i < 5; i++)
	{
		cFelorcAxe* pFelOrcAxe = new cFelorcAxe;
		pFelOrcAxe->Setup();
		m_vecMonster.push_back(pFelOrcAxe);
	}

	for (int i = 0; i < 5; i++)
	{
		cFelorcDire* pFelOrcDire = new cFelorcDire;
		pFelOrcDire->Setup();
		m_vecMonster.push_back(pFelOrcDire);
	}
	for (int i = 0; i < 5; i++)
	{
		cFelorcSword* pFelOrcSword = new cFelorcSword;
		pFelOrcSword->Setup();
		m_vecMonster.push_back(pFelOrcSword);
	}
	for (int i = 0; i < 5; i++)
	{
		cFelorcWarriorSword* pFelOrcWarriorSword = new cFelorcWarriorSword;
		pFelOrcWarriorSword->Setup();
		m_vecMonster.push_back(pFelOrcWarriorSword);
	}

	for (int i = 0; i < g_pGameManager->GetStageOneTile()->GetTileInfo().size(); i++)
	{
		if (g_pGameManager->GetStageOneTile()->GetTileInfo()[i].type == ST_TILE_INFO::MONSTER_SPAWN_BLOCK)
			g_pGameManager->GetStageOneTile()->GetTileInfo()[i].type = ST_TILE_INFO::MONSTER_SPAWN;
	}

	cCharacter* character = new cCharacter;
	character->Setup();
	m_vecCharacter.push_back(character);

	for each(cFelorcAxe* p in m_vecMonster)
		p->ThreadResume();
}

void cGameManager::StageOneUpdate()
{
	for each(cMonster* monster in m_vecMonster)
		monster->Update();

	for each(cCharacter* character in m_vecCharacter)
		character->Update();
}

void cGameManager::StageOneRender()
{
	/*g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pStageOneTile->Render();
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);*/

	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	for each(cMonster* monster in m_vecMonster)
		monster->Render();

	for each(cCharacter* character in m_vecCharacter)
		character->Render();

	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
}


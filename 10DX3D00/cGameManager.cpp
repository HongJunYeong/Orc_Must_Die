#include "stdafx.h"
#include "cGameManager.h"

#include "cMonster.h"
#include "cTile.h"

#include "cMonster.h"
#include "cFelorcAxe.h"

cGameManager::cGameManager()
	:m_sPlayerName("")
	,m_pStageOneTile(NULL)
{
}


cGameManager::~cGameManager()
{
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

void cGameManager::StageOneSetup()
{
	m_pStageOneTile = new cTile;

	m_pStageOneTile->Setup(300, 1.0f);

	cFelorcAxe* pFelOrcAxe = new cFelorcAxe;
	pFelOrcAxe->Setup();

	m_vecMonster.push_back(pFelOrcAxe);
}

void cGameManager::StageOneUpdate()
{
	for each(cMonster* monster in m_vecMonster)
		monster->Update();
}

void cGameManager::StageOneRender()
{
	m_pStageOneTile->Render();

	for each(cMonster* monster in m_vecMonster)
		monster->Render();
}


#include "stdafx.h"
#include "cGameManager.h"

#include "cMonster.h"
#include "cTile.h"

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
}


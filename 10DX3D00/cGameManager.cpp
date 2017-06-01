#include "stdafx.h"
#include "cGameManager.h"


cGameManager::cGameManager()
	:m_sPlayerName("")
{
}


cGameManager::~cGameManager()
{
}

void cGameManager::SetPlayerName(string name)
{
	m_sPlayerName = name;
}

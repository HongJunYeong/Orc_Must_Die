#pragma once

#define g_pGameManager cGameManager::GetInstance() 

class cMonster;
class cTile;

class cGameManager
{
public:

private:
	SINGLETONE(cGameManager);

	string				m_sPlayerName;
	vector<cMonster*>	m_vecMonster;

	cTile*				m_pStageOneTile;
public:
	void SetPlayerName(string name);
	string GetPlayerName();

	void StageOneSetup();
};


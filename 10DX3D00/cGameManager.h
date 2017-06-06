#pragma once

#define g_pGameManager cGameManager::GetInstance() 

class cMonster;

class cGameManager
{
public:

private:
	SINGLETONE(cGameManager);

	string				m_sPlayerName;
	vector<cMonster>	m_vecMonster;
public:
	void SetPlayerName(string name);
	string GetPlayerName();

	void StageOneSetup();
};


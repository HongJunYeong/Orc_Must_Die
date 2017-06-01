#pragma once

#define g_pGameManager cGameManager::GetInstance() 

class cGameManager
{
public:

private:
	SINGLETONE(cGameManager);

	string		m_sPlayerName;
public:
	void SetPlayerName(string name);
};


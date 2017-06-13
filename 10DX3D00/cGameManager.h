#pragma once

#define g_pGameManager cGameManager::GetInstance() 

class cMonster;
class cCharacter;
class cTile;

class cGameManager
{
public:

private:
	SINGLETONE(cGameManager);

	string				m_sPlayerName;
	vector<cMonster*>	m_vecMonster;
	vector<cCharacter*>	m_vecCharacter;

	cTile*				m_pStageOneTile;
public:
	HANDLE				m_hSem;

	void Destroy();
	void SetPlayerName(string name);
	string GetPlayerName();
	cTile* GetStageOneTile();

	vector<cMonster*>& GetMonster()
	{
		return m_vecMonster;
	}

	void StageOneTileSetup();

	void StageOneSetup();
	void StageOneUpdate();
	void StageOneRender();

	vector<cCharacter*>& GetCharacter() { return m_vecCharacter; }
};


#pragma once

#define g_pSceneManager cSceneManager::GetInstance()

class cScene;

class cSceneManager
{
private:
	SINGLETONE(cSceneManager);

	map<string, cScene*> m_mapScene;
	cScene* m_pCurrScene;
public:
	void Update();
	void Render(LPD3DXSPRITE pSprite);
	void SetCurrentScene(string sSceneName);
	void AddScene(string sSceneName, cScene* pScene);
	cScene* GetCurrentScene();
	void Destroy();
};


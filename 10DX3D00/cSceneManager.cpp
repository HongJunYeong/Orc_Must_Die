#include "stdafx.h"
#include "cSceneManager.h"

#include "cScene.h"

cSceneManager::cSceneManager()
	:m_pCurrScene(NULL)
{
}


cSceneManager::~cSceneManager()
{
}

void cSceneManager::Update()
{
	if (m_pCurrScene)
		m_pCurrScene->Update();
}

void cSceneManager::Render(LPD3DXSPRITE pSprite)
{
	if (m_pCurrScene)
		m_pCurrScene->Render(pSprite);
}

void cSceneManager::SetCurrentScene(string sSceneName)
{
	if (m_pCurrScene)
		m_pCurrScene->CloseThread();

	m_pCurrScene = m_mapScene[sSceneName];
}

void cSceneManager::AddScene(string sSceneName, cScene * pScene)
{
	m_mapScene.insert(pair<string, cScene*>(sSceneName, pScene));
}

cScene * cSceneManager::GetCurrentScene()
{
	return m_pCurrScene;
}

void cSceneManager::Destroy()
{
	for each(auto p in m_mapScene)
	{
		p.second->Release();
	}
}

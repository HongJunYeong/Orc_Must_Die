#include "stdafx.h"
#include "cMainGame.h"
#include "cDeviceManager.h"

// >> :
#include "cCamera.h"
#include "cGrid.h"
#include "cTitleScene.h"
#include "cMenuScene.h"
#include "cCharacterNameScene.h"
#include "cSetIPScene.h"
#include "cMultiReadyScene.h"
// << :


cMainGame::cMainGame()
	: m_pCamera(NULL)
	, m_pGrid(NULL)
{
}


cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);

	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_cursor);
	SAFE_RELEASE(m_surf);

	//�Ŵ����� ����
	g_pNetworkManager->Destroy();

	g_pSoundManager->Destroy();
	g_pFontManager->Destroy();
	g_pObjectManager->Destroy();
	g_pTextureManager->Destroy();
	g_pSceneManager->Destroy();

	g_pDeviceManager->Destroy();
}

// >> : 
void cMainGame::Setup()
{
	// >> : �ӽ� �����Դϴ�.
	D3DLIGHT9 stLight;
	{
		ZeroMemory(&stLight, sizeof(D3DLIGHT9));
		stLight.Type = D3DLIGHT_DIRECTIONAL;
		stLight.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		stLight.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		stLight.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		D3DXVECTOR3 vDir(1.0f, -1.0f, 1.0f);
		D3DXVec3Normalize(&vDir, &vDir);
		stLight.Direction = vDir;
		g_pD3DDevice->SetLight(0, &stLight);
	} // << �ӽ� ����

	m_pCamera = new cCamera;
	m_pCamera->Setup(NULL);

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	// >>
	g_pSoundManager->addSound(std::string("1"), std::string("S0455.mp3"), false, false);

	// <<

	D3DXCreateSprite(
		g_pD3DDevice,
		&m_pSprite
	);

	m_cursor = g_pTextureManager->GetTexture("Image/����/swordCursor.bmp");
	m_cursor->GetSurfaceLevel(0, &m_surf);


	g_pD3DDevice->SetCursorProperties(0, 0, m_surf);

	g_pD3DDevice->LightEnable(0, true);
	
	cTitleScene* pTitleScene = new cTitleScene;
	pTitleScene->Setup();
	g_pSceneManager->AddScene("TitleScene", pTitleScene);
	g_pSceneManager->SetCurrentScene("TitleScene");


	cMenuScene* pMenuScene = new cMenuScene;
	pMenuScene->Setup();
	g_pSceneManager->AddScene("MenuScene", pMenuScene);

	cCharacterNameScene* pCharacterNameScene = new cCharacterNameScene;
	pCharacterNameScene->Setup();
	g_pSceneManager->AddScene("CharacterNameScene", pCharacterNameScene);

	cSetIPScene* pSetIPScene = new cSetIPScene;
	pSetIPScene->Setup();
	g_pSceneManager->AddScene("SetIPScene", pSetIPScene);

	cMultiReadyScene* pMultiReadyScene = new cMultiReadyScene;
	pMultiReadyScene->Setup();
	g_pSceneManager->AddScene("MultiReadyScene", pMultiReadyScene);

}

void cMainGame::Update()
{

	if (m_pCamera) m_pCamera->Update();

	g_pSceneManager->Update();

	//if (g_pKeyManager->isOnceKeyDown('A'))
	//{
		// << Ű�Ŵ��� �۵�Ȯ��
		//MessageBeep(0);


		//����Ŵ��� �۵�Ȯ��
		//g_pSoundManager->play(std::string("1"), 1.0f);
	//}
	//if (g_pKeyManager->isOnceKeyDown('D'))
	//{
	//	g_pSoundManager->stop(std::string("1"));
	//}

}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(47, 121, 112), 1.0f, 0);

	g_pD3DDevice->BeginScene();


	//if (m_pGrid) m_pGrid->Render();
	g_pSceneManager->Render(m_pSprite);


	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
// << : 
void cMainGame::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hwnd, message, wParam, lParam); 
	}
}
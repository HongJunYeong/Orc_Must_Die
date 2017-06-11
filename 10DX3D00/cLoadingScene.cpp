#include "stdafx.h"
#include "cLoadingScene.h"

#include "cUIImageView.h"
#include "cStageOneScene.h"

unsigned int __stdcall LoadingThread(LPVOID p);

cLoadingScene::cLoadingScene()
{
}


cLoadingScene::~cLoadingScene()
{
}

void cLoadingScene::Setup()
{
	Setup_UI();
	m_isLoadSuccess = false;
	m_fGearRotZ = 0.0f;
	m_hLoadThread = (HANDLE)_beginthreadex(NULL, 0, LoadingThread, this, CREATE_SUSPENDED, NULL);
	cScene::Setup();
}

void cLoadingScene::Update()
{
	if (m_nScreenChangeAlpha == 250)
	{
		ResumeThread(m_hLoadThread);
	}

	m_fGearRotZ += 0.01f;
	m_pGearImage->SetRotZ(m_fGearRotZ);

	if (m_pGearImage)
		m_pGearImage->Update();

	cScene::Update();

	cout << GetTickCount() << endl;
}

void cLoadingScene::Render(LPD3DXSPRITE pSprite)
{

	if (m_pLoadImage)
		m_pLoadImage->Render(pSprite);

	if (m_pGearImage)
		m_pGearImage->Render(pSprite);

	cScene::Render(pSprite);
}

void cLoadingScene::Release()
{
	CloseHandle(m_hLoadThread);
	m_pLoadImage->Release();
	m_pGearImage->Release();
}

void cLoadingScene::StartRecvThread()
{
}

void cLoadingScene::CloseThread()
{
}

void cLoadingScene::Loading()
{
	g_pGameManager->StageOneSetup();
	g_pSceneManager->SetCurrentScene("StageOneScene");
}

void cLoadingScene::Setup_UI()
{
	m_pLoadImage = new cUIImageView;

	m_pLoadImage->SetPosition(0, 0);
	m_pLoadImage->SetTexture("Image/공용/LoadImage1.dds");

	m_pGearImage = new cUIImageView;

	m_pGearImage->SetPosition(1200, 640);
	m_pGearImage->SetTexture("Image/공용/gear.dds");
	m_pGearImage->SetisRotZMode(true);
	m_pGearImage->m_fCenterX = 40.0f;
	m_pGearImage->m_fCenterY = 40.0f;
}

unsigned int __stdcall LoadingThread(LPVOID p)
{
	cLoadingScene* g = (cLoadingScene*)p;
	g->Loading();
	return 0;
}

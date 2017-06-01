#include "stdafx.h"
#include "cTitleScene.h"

#include "cUIObject.h"
#include "cUIImageView.h"
#include "cUIButton.h"

cTitleScene::cTitleScene()
	:m_pUIRoot(NULL)
	,m_nPressAnykeyAlpha(255)
	,m_isPressKeyAlphaUp(false)
{
}


cTitleScene::~cTitleScene()
{
}

void cTitleScene::Setup()
{
	Setup_UI();

	cScene::Setup();
}

void cTitleScene::Update()
{
	g_pTimeManager->Update();

	if (m_pUIRoot)
		m_pUIRoot->Update();

	if (g_pKeyManager->isOnceKeyDown(VK_RETURN))
	{
		g_pSceneManager->SetCurrentScene("CharacterNameScene");
		m_isSceneChange = false;
	}
	
	m_pUIPressAnyKeyImage->SetAlpha(m_nPressAnykeyAlpha);

	if (m_isPressKeyAlphaUp)
		m_nPressAnykeyAlpha+=5;
	else
		m_nPressAnykeyAlpha-=5;

	if (m_nPressAnykeyAlpha <= 0)
		m_isPressKeyAlphaUp = true;
	else if (m_nPressAnykeyAlpha >= 255)
		m_isPressKeyAlphaUp = false;

	cScene::Update();
}

void cTitleScene::Render(LPD3DXSPRITE pSprite)
{
	if (m_pUIRoot)
		m_pUIRoot->Render(pSprite);

	cScene::Render(pSprite);
}

void cTitleScene::Release()
{
	m_pUIRoot->Destroy();
	m_pUIPressAnyKeyImage->Destroy();
}

void cTitleScene::Setup_UI()
{
	cUIImageView* cTitleImage = new cUIImageView;
	cTitleImage->SetPosition(0, 0);
	cTitleImage->SetTexture("Image/TitleScene/banner.png");

	m_pUIRoot = cTitleImage;

	m_pUIPressAnyKeyImage = new cUIImageView;
	m_pUIPressAnyKeyImage->SetPosition(390, 450);
	m_pUIPressAnyKeyImage->SetTexture("Image/TitleScene/PressAnyKey.png");

	cTitleImage->AddChild(m_pUIPressAnyKeyImage);
}


#include "stdafx.h"
#include "cMenuScene.h"

#include "cUIImageView.h"
#include "cUIButton.h"

cMenuScene::cMenuScene()
{
}


cMenuScene::~cMenuScene()
{
}

void cMenuScene::Setup()
{
	Setup_UI();
	cScene::Setup();
}

void cMenuScene::Update()
{
	if (m_pUIRoot)
		m_pUIRoot->Update();

	m_pBtnBgImage->SetisHidden(true);

	for (int i = 0; i < m_vecBtn.size(); i++)
	{
		if (m_vecBtn[i]->GetButtonState() == cUIButton::E_MOUSEOVER
			|| m_vecBtn[i]->GetButtonState() == cUIButton::E_SELECTED)
		{
			m_pBtnBgImage->SetPosition(350, m_vecBtn[i]->GetPosition().y - 20);
			m_pBtnBgImage->SetisHidden(false);
			break;
		}
	}

	cScene::Update();
}

void cMenuScene::Render(LPD3DXSPRITE pSprite)
{
	if (m_pUIRoot)
		m_pUIRoot->Render(pSprite);

	cScene::Render(pSprite);
}

void cMenuScene::Release()
{
	m_pUIRoot->Destroy();
	m_pBtnBgImage->Destroy();
}

void cMenuScene::Setup_UI()
{
	cUIImageView* pUIMenuBGImage = new cUIImageView;
	pUIMenuBGImage->SetPosition(0, 0);
	pUIMenuBGImage->SetTexture("Image/MenuScene/MenuImage2.dds");

	m_pUIRoot = pUIMenuBGImage;

	cUIImageView* pIconImage = new cUIImageView;
	pIconImage->SetPosition(510, 50);
	pIconImage->SetTexture("Image/MenuScene/MenuIcon.dds");

	cUIButton* pSingleBtn = new cUIButton;
	pSingleBtn->SetTexture("Image/MenuScene/singleBtnNor.png",
		"Image/MenuScene/singleBtnOver.png",
		"Image/MenuScene/singleBtnOver.png");
	pSingleBtn->SetPosition(560, 300);

	cUIButton* pMultiBtn = new cUIButton;
	pMultiBtn->SetTexture("Image/MenuScene/multiBtnNor.png",
		"Image/MenuScene/multiBtnOver.png",
		"Image/MenuScene/multiBtnOver.png");
	pMultiBtn->SetPosition(560, 380);
	pMultiBtn->m_OnClick = bind(&cMenuScene::OnClickMutiPlay, this);

	cUIButton* pOptionBtn = new cUIButton;
	pOptionBtn->SetTexture("Image/MenuScene/OptionBtnNor.png",
		"Image/MenuScene/OptionBtnOver.png",
		"Image/MenuScene/OptionBtnOver.png");
	pOptionBtn->SetPosition(560, 460);

	cUIButton* pExitBtn = new cUIButton;
	pExitBtn->SetTexture("Image/MenuScene/ExitBtnNor.png",
		"Image/MenuScene/ExitBtnOver.png",
		"Image/MenuScene/ExitBtnOver.png");

	pExitBtn->SetPosition(560, 540);
	pExitBtn->m_OnClick = std::bind(&cMenuScene::OnClickExit, this);

	m_pBtnBgImage = new cUIImageView;
	m_pBtnBgImage->SetPosition(400, 540);
	m_pBtnBgImage->SetTexture("Image/°ø¿ë/BtnBg.png");
	m_pBtnBgImage->Update();
	m_pBtnBgImage->SetisHidden(true);

	pUIMenuBGImage->AddChild(pIconImage);
	pUIMenuBGImage->AddChild(m_pBtnBgImage);
	pUIMenuBGImage->AddChild(pSingleBtn);
	pUIMenuBGImage->AddChild(pMultiBtn);
	pUIMenuBGImage->AddChild(pOptionBtn);
	pUIMenuBGImage->AddChild(pExitBtn);

	m_vecBtn.push_back(pSingleBtn);
	m_vecBtn.push_back(pMultiBtn);
	m_vecBtn.push_back(pOptionBtn);
	m_vecBtn.push_back(pExitBtn);
}

void cMenuScene::OnClickMutiPlay()
{
	g_pSceneManager->SetCurrentScene("SetIPScene");
	m_isSceneChange = true;
	m_nScreenChangeAlpha = 255;
}

void cMenuScene::OnClickExit()
{
	PostMessage(g_hWnd, WM_QUIT, 0, 0);
}

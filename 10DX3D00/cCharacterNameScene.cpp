#include "stdafx.h"
#include "cCharacterNameScene.h"

#include "cUIButton.h"
#include "cUIImageView.h"
#include "cUITextView.h"

cCharacterNameScene::cCharacterNameScene()
{
}


cCharacterNameScene::~cCharacterNameScene()
{
}

void cCharacterNameScene::Setup()
{
	cScene::Setup();

	Setup_UI();
}

void cCharacterNameScene::Update()
{
	if (m_nScreenChangeAlpha == 200)
	{
		m_hNameEdit = CreateWindow("edit", NULL, WS_CHILD | WS_BORDER | WS_VISIBLE | ES_CENTER,
			715, 360, 300, 30, g_hWnd, (HMENU)0, g_hInstance, NULL);
		m_hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET,
			3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("HY목각파임B"));
		SendMessage(m_hNameEdit, WM_SETFONT, (WPARAM)m_hFont, MAKELPARAM(FALSE, 0));
	}

	if (m_pUIRoot)
		m_pUIRoot->Update();

	if (g_pKeyManager->isOnceKeyDown(VK_RETURN))
		OnClickNameSave();

	cScene::Update();
}

void cCharacterNameScene::Render(LPD3DXSPRITE pSprite)
{
	if (m_pUIRoot)
		m_pUIRoot->Render(pSprite);

	cScene::Render(pSprite);
}

void cCharacterNameScene::Release()
{
	m_pUIRoot->Destroy();
}

void cCharacterNameScene::Setup_UI()
{
	cUIImageView* pBgImg = new cUIImageView;
	pBgImg->SetPosition(0, 0);
	pBgImg->SetTexture("Image/CharacterNameScene/BG.dds");

	m_pUIRoot = pBgImg;

	cUIImageView* pBarImg = new cUIImageView;
	pBarImg->SetPosition(600, 150);
	pBarImg->SetTexture("Image/CharacterNameScene/PlayerName.dds");

	cUIButton* pGoBtn = new cUIButton;
	pGoBtn->SetPosition(1000, 480);
	pGoBtn->SetTexture("Image/공용/GoBtnSel.dds",
		"Image/공용/GoBtnSel.dds",
		"Image/공용/GoBtnSel.dds");
	pGoBtn->m_OnClick = bind(&cCharacterNameScene::OnClickNameSave, this);
	pGoBtn->SetScaleMode(true);

	cUITextView* pNameText = new cUITextView;
	pNameText->SetText("플레이어 이름을 정해주세요!");
	pNameText->SetSize(ST_SIZEN(300, 200));
	pNameText->SetPosition(710, 200);
	pNameText->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	pNameText->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	pNameText->SetFontType(cFontManager::E_WOOD_DUG);
	pNameText->SetTag(10);

	pBgImg->AddChild(pGoBtn);
	pBgImg->AddChild(pBarImg);
	pBgImg->AddChild(pNameText);
}

void cCharacterNameScene::OnClickNameSave()
{
	char str[256];
	GetWindowText(m_hNameEdit, str, 256);

	string name(str);

	cout << name << endl;
	if (name == "")
	{
		cUITextView* textView = (cUITextView*)m_pUIRoot->FindChildByTag(10);
		textView->SetText("플레이어 이름을 반드시 입력해주세요!");
		return;
	}

	g_pGameManager->SetPlayerName(name);
	DestroyWindow(m_hNameEdit);

	g_pSceneManager->SetCurrentScene("MenuScene");
	m_isSceneChange = false;

	DeleteObject(m_hFont);
}

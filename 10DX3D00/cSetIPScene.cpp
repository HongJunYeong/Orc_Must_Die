#include "stdafx.h"
#include "cSetIPScene.h"

#include "cUIObject.h"
#include "cUIImageView.h"
#include "cUITextView.h"
#include "cUIButton.h"

cSetIPScene::cSetIPScene()
{
}


cSetIPScene::~cSetIPScene()
{
}

void cSetIPScene::Setup()
{
	cScene::Setup();

	Setup_UI();
}

void cSetIPScene::Update()
{
	if (m_pUIRoot)
		m_pUIRoot->Update();

	if (g_pKeyManager->isOnceKeyDown(VK_RETURN))
		OnClickNextScene();

	cScene::Update();
}

void cSetIPScene::Render(LPD3DXSPRITE pSprite)
{
	if (m_nScreenChangeAlpha == 200)
	{
		m_hNameEdit = CreateWindow("edit", NULL, WS_CHILD | WS_BORDER | WS_VISIBLE | ES_CENTER,
			450, 390, 400, 40, g_hWnd, (HMENU)0, g_hInstance, NULL);
		m_hFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET,
			3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("HY목각파임B"));
		SendMessage(m_hNameEdit, WM_SETFONT, (WPARAM)m_hFont, MAKELPARAM(FALSE, 0));
	}

	if (m_pUIRoot)
		m_pUIRoot->Render(pSprite);

	cScene::Render(pSprite);
}

void cSetIPScene::Release()
{
	m_pUIRoot->Release();
}

void cSetIPScene::Setup_UI()
{
	cUIImageView* pBgImg = new cUIImageView;
	pBgImg->SetPosition(0, 0);
	pBgImg->SetTexture("Image/SetIPScene/BG.jpg");

	m_pUIRoot = pBgImg;

	cUIImageView* pPannelImg = new cUIImageView;
	pPannelImg->SetPosition(240, 70);
	pPannelImg->SetTexture("Image/SetIPScene/multiPannel.dds");

	cUIButton* pBackBtn = new cUIButton;
	pBackBtn->SetPosition(70, 480);
	pBackBtn->SetTexture("Image/공용/BackBtnNor.dds",
		"Image/공용/BackBtnSel.dds",
		"Image/공용/BackBtnSel.dds");
	pBackBtn->m_OnClick = bind(&cSetIPScene::OnClickPrevScene, this);

	cUIButton* pGoBtn = new cUIButton;
	pGoBtn->SetPosition(1000, 490);
	pGoBtn->SetTexture("Image/공용/GoBtnSel.dds",
		"Image/공용/GoBtnSel.dds",
		"Image/공용/GoBtnSel.dds");
	pGoBtn->m_OnClick = bind(&cSetIPScene::OnClickNextScene, this);
	pGoBtn->SetScaleMode(true);

	cUITextView* pIPText = new cUITextView;
	pIPText->SetText("접속할 서버의 IP주소를 적어주세요.!");
	pIPText->SetSize(ST_SIZEN(300, 200));
	pIPText->SetPosition(500, 200);
	pIPText->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	pIPText->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	pIPText->SetFontType(cFontManager::E_WOOD_DUG);
	pIPText->SetTag(10);

	m_pUIRoot->AddChild(pPannelImg);
	m_pUIRoot->AddChild(pIPText);
	m_pUIRoot->AddChild(pBackBtn);
	m_pUIRoot->AddChild(pGoBtn);
}

void cSetIPScene::OnClickNextScene()
{
	char str[256];
	GetWindowText(m_hNameEdit, str, 256);

	string sIP(str);

	cout << sIP << endl;

	if (sIP == "")
	{
		cUITextView* textView = (cUITextView*)m_pUIRoot->FindChildByTag(10);
		textView->SetText("접속할 서버의 IP를 반드시 입력해주세요.!");
		return;
	}

	g_pNetworkManager->SetServerIP(sIP);
	if (!g_pNetworkManager->Setup())
	{
		cUITextView* textView = (cUITextView*)m_pUIRoot->FindChildByTag(10);
		textView->SetText("서버에 접속하실수 없습니다.!");
		return;
	}

	g_pSceneManager->SetCurrentScene("MultiReadyScene");
	g_pSceneManager->GetCurrentScene()->StartRecvThread();

	ST_NETWORK stNet;
	stNet.eNetType = E_SET_MY_NETWORK_ID;
	stNet.sPlayerName = g_pGameManager->GetPlayerName();


	send(g_pNetworkManager->GetSocket(), (char*)&stNet, sizeof(stNet), 0);

	m_isSceneChange = true;
	m_nScreenChangeAlpha = 255;

	DestroyWindow(m_hNameEdit);
	DeleteObject(m_hFont);

	while (!g_pNetworkManager->GetisMineSetting())
	{

	}
}

void cSetIPScene::OnClickPrevScene()
{
	m_isSceneChange = true;
	m_nScreenChangeAlpha = 255;

	DestroyWindow(m_hNameEdit);
	DeleteObject(m_hFont);

	g_pSceneManager->SetCurrentScene("MenuScene");
}

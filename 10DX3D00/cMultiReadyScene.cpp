#include "stdafx.h"
#include "cMultiReadyScene.h"

#include "cUIButton.h"
#include "cUIImageView.h"
#include "cUITextView.h"
#include "cChatLog.h"

unsigned int __stdcall RecvThread(LPVOID p);

cMultiReadyScene::cMultiReadyScene()
{
}


cMultiReadyScene::~cMultiReadyScene()
{
}

void cMultiReadyScene::Setup()
{
	Setup_UI();
	g_pNetworkManager->m_funcRefreshRoom = bind(&cMultiReadyScene::RefreshRoom, this);
	//g_pNetworkManager->m_funcRogUpdate = bind(&cMultiReadyScene::LogUpdate, this,placeholders::_1);
	m_pLog = NULL;
	cScene::Setup();
}

void cMultiReadyScene::Update()
{
	if (m_pUIRoot)
		m_pUIRoot->Update();
	cScene::Update();
}

void cMultiReadyScene::Render(LPD3DXSPRITE pSprite)
{
	if (m_pUIRoot)
		m_pUIRoot->Render(pSprite);

	cScene::Render(pSprite);

	//m_pLog->Render(pSprite);
}

void cMultiReadyScene::Release()
{
	m_pUIRoot->Destroy();
	//m_pLog->Destroy();
}

void cMultiReadyScene::Setup_UI()
{
	cUIImageView* pBGImg = new cUIImageView;
	pBGImg->SetPosition(0, 0);
	pBGImg->SetTexture("Image/MultiReadyScene/BG.png");

	m_pUIRoot = pBGImg;

	cUIImageView* pReadyBar = new cUIImageView;
	pReadyBar->SetPosition(400, -10);
	pReadyBar->SetTexture("Image/����/SmallBar2.dds");

	cUIButton* pReadyBtn = new cUIButton;
	pReadyBtn->SetPosition(490, -50);
	pReadyBtn->SetTexture("Image/MultiReadyScene/ReadyBtnNor.dds",
		"Image/MultiReadyScene/ReadyBtnSel.dds",
		"Image/MultiReadyScene/ReadyBtnSel.dds");
	pReadyBtn->SetScaleMode(true);
	pReadyBtn->m_OnClick = bind(&cMultiReadyScene::OnClickReady, this);

	cUIImageView* pUserInfoImg1 = new cUIImageView;
	pUserInfoImg1->SetPosition(-30, 500);
	pUserInfoImg1->SetTexture("Image/MultiReadyScene/UserInfo.dds");

	cUIImageView* pUserInfoImg2 = new cUIImageView;
	pUserInfoImg2->SetPosition(300, 500);
	pUserInfoImg2->SetTexture("Image/MultiReadyScene/UserInfo.dds");

	cUIImageView* pUserInfoImg3 = new cUIImageView;
	pUserInfoImg3->SetPosition(620, 500);
	pUserInfoImg3->SetTexture("Image/MultiReadyScene/UserInfo.dds");

	cUIImageView* pUserInfoImg4 = new cUIImageView;
	pUserInfoImg4->SetPosition(930, 500);
	pUserInfoImg4->SetTexture("Image/MultiReadyScene/UserInfo.dds");

	cUITextView* p1PText = new cUITextView;
	p1PText->SetText("");
	p1PText->SetSize(ST_SIZEN(300, 200));
	p1PText->SetPosition(30, -10);
	p1PText->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	p1PText->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	p1PText->SetFontType(cFontManager::E_WOOD_DUG);
	p1PText->SetTag(E_TEXT_1P_NAME);

	cUITextView* p2PText = new cUITextView;
	p2PText->SetText("");
	p2PText->SetSize(ST_SIZEN(300, 200));
	p2PText->SetPosition(30, -10);
	p2PText->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	p2PText->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	p2PText->SetFontType(cFontManager::E_WOOD_DUG);
	p2PText->SetTag(E_TEXT_2P_NAME);

	cUITextView* p3PText = new cUITextView;
	p3PText->SetText("");
	p3PText->SetSize(ST_SIZEN(300, 200));
	p3PText->SetPosition(30, -10);
	p3PText->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	p3PText->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	p3PText->SetFontType(cFontManager::E_WOOD_DUG);
	p3PText->SetTag(E_TEXT_3P_NAME);

	cUITextView* p4PText = new cUITextView;
	p4PText->SetText("");
	p4PText->SetSize(ST_SIZEN(300, 200));
	p4PText->SetPosition(30, -10);
	p4PText->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	p4PText->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	p4PText->SetFontType(cFontManager::E_WOOD_DUG);
	p4PText->SetTag(E_TEXT_4P_NAME);

	cUITextView* p1PReadyText = new cUITextView;
	p1PReadyText->SetText("");
	p1PReadyText->SetSize(ST_SIZEN(300, 200));
	p1PReadyText->SetPosition(0, 60);
	p1PReadyText->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	p1PReadyText->SetTextColor(D3DCOLOR_XRGB(200, 0, 0));
	p1PReadyText->SetFontType(cFontManager::E_QUEST);
	p1PReadyText->SetTag(E_TEXT_1P_READY);

	cUITextView* p2PReadyText = new cUITextView;
	p2PReadyText->SetText("");
	p2PReadyText->SetSize(ST_SIZEN(300, 200));
	p2PReadyText->SetPosition(0, 60);
	p2PReadyText->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	p2PReadyText->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	p2PReadyText->SetFontType(cFontManager::E_QUEST);
	p2PReadyText->SetTag(E_TEXT_2P_READY);

	cUITextView* p3PReadyText = new cUITextView;
	p3PReadyText->SetText("");
	p3PReadyText->SetSize(ST_SIZEN(300, 200));
	p3PReadyText->SetPosition(0, 60);
	p3PReadyText->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	p3PReadyText->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	p3PReadyText->SetFontType(cFontManager::E_QUEST);
	p3PReadyText->SetTag(E_TEXT_3P_READY);

	cUITextView* p4PReadyText = new cUITextView;
	p4PReadyText->SetText("");
	p4PReadyText->SetSize(ST_SIZEN(300, 200));
	p4PReadyText->SetPosition(0, 60);
	p4PReadyText->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	p4PReadyText->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	p4PReadyText->SetFontType(cFontManager::E_QUEST);
	p4PReadyText->SetTag(E_TEXT_4P_READY);

	pBGImg->AddChild(pReadyBar);
	pBGImg->AddChild(pReadyBtn);
	pBGImg->AddChild(pUserInfoImg1);
	pBGImg->AddChild(pUserInfoImg2);
	pBGImg->AddChild(pUserInfoImg3);
	pBGImg->AddChild(pUserInfoImg4);

	pUserInfoImg1->AddChild(p1PText);
	pUserInfoImg2->AddChild(p2PText);
	pUserInfoImg3->AddChild(p3PText);
	pUserInfoImg4->AddChild(p4PText);

	p1PText->AddChild(p1PReadyText);
	p2PText->AddChild(p2PReadyText);
	p3PText->AddChild(p3PReadyText);
	p4PText->AddChild(p4PReadyText);
}

void cMultiReadyScene::RefreshRoom()
{
	cUITextView* view1 = (cUITextView*)m_pUIRoot->FindChildByTag(E_TEXT_1P_NAME);
	view1->SetText("");

	cUITextView* view2 = (cUITextView*)m_pUIRoot->FindChildByTag(E_TEXT_2P_NAME);
	view2->SetText("");

	cUITextView* view3 = (cUITextView*)m_pUIRoot->FindChildByTag(E_TEXT_3P_NAME);
	view3->SetText("");

	cUITextView* view4 = (cUITextView*)m_pUIRoot->FindChildByTag(E_TEXT_4P_NAME);
	view4->SetText("");

	cUITextView* viewReady1 = (cUITextView*)m_pUIRoot->FindChildByTag(E_TEXT_1P_READY);
	viewReady1->SetText("");

	cUITextView* viewReady2 = (cUITextView*)m_pUIRoot->FindChildByTag(E_TEXT_2P_READY);
	viewReady2->SetText("");

	cUITextView* viewReady3 = (cUITextView*)m_pUIRoot->FindChildByTag(E_TEXT_3P_READY);
	viewReady3->SetText("");

	cUITextView* viewReady4 = (cUITextView*)m_pUIRoot->FindChildByTag(E_TEXT_4P_READY);
	viewReady4->SetText("");

	view1->SetText(g_pGameManager->GetPlayerName());

	if (g_pNetworkManager->GetMine().isReady)
	{
		viewReady1->SetText("READY");
	}


	for (int i = 0; i < g_pNetworkManager->GetNetworkPlayer().size(); i++)
	{
		if (i == 0)
		{
			view2->SetText(g_pNetworkManager->GetNetworkPlayer()[i].sPlayerName);
			if (g_pNetworkManager->GetNetworkPlayer()[i].isReady)
			{
				viewReady2->SetText("READY");
			}
		}
		if (i == 1)
		{
			view3->SetText(g_pNetworkManager->GetNetworkPlayer()[i].sPlayerName);
			if (g_pNetworkManager->GetNetworkPlayer()[i].isReady)
			{
				viewReady3->SetText("READY");
			}
		}
		if (i == 2)
		{
			view4->SetText(g_pNetworkManager->GetNetworkPlayer()[i].sPlayerName);
			if (g_pNetworkManager->GetNetworkPlayer()[i].isReady)
			{
				viewReady4->SetText("READY");
			}
		}
	}

	//m_pLog->AddLOG(g_pNetworkManager->GetLogin());
}

void cMultiReadyScene::OnClickReady()
{

	g_pNetworkManager->SetReady(true);

	ST_NETWORK stNet;
	stNet.isReady = true;
	stNet.eNetType = E_READY;
	stNet.nNetID = g_pNetworkManager->GetNetId();
	stNet.sPlayerName = g_pGameManager->GetPlayerName();

	send(g_pNetworkManager->GetSocket(), (char*)&stNet, sizeof(stNet), 0);
}

void cMultiReadyScene::StartRecvThread()
{
	int nThreadId = 1;
	m_hRecvThread = (HANDLE)_beginthreadex(NULL, 0, RecvThread, this, 0, (unsigned*)&nThreadId);
}

void cMultiReadyScene::LogUpdate(string sLog)
{
	m_pLog->AddLOG(sLog);
}

void cMultiReadyScene::RecvNetwork()
{
}

unsigned int __stdcall RecvThread(LPVOID p)
{
	cMultiReadyScene* g = (cMultiReadyScene*)p;
	g->RecvNetwork();

	return 0;
}

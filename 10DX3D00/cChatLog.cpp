#include "stdafx.h"
#include "cChatLog.h"

#include "cUITextView.h"

cChatLog::cChatLog()
{
}


cChatLog::~cChatLog()
{
}

void cChatLog::Update()
{
	for each(cUITextView* p in m_vecLog)
	{
		p->Update();
	}
}

void cChatLog::Render(LPD3DXSPRITE pSprite)
{
	for each(cUITextView* p in m_vecLog)
	{
		p->Render(pSprite);
	}
}

void cChatLog::Destroy()
{
	for each(cUITextView* p in m_vecLog)
		p->Destroy();

	m_vecLog.clear();
}

void cChatLog::AddLOG(string sLog)
{
	if (m_vecLog.size() == 8)
	{
		m_vecLog.erase(m_vecLog.begin());
	}

	cUITextView* pLog = new cUITextView;
	pLog->SetPosition(30, 400);
	pLog->SetText(sLog);
	pLog->SetSize(ST_SIZEN(700, 200));
	pLog->SetDrawTextFormat(DT_LEFT | DT_VCENTER | DT_WORDBREAK);
	pLog->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	pLog->SetFontType(cFontManager::E_WOOD_DUG);

	for each(cUITextView* p in m_vecLog)
	{
		p->SetPosition(p->GetPosition().x, p->GetPosition().y - 30.0f);
	}

	m_vecLog.push_back(pLog);
}

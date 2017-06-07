#pragma once

class cUITextView;

class cChatLog
{
public:
	cChatLog();
	~cChatLog();

private:
	vector<cUITextView*> m_vecLog;

public:
	void Update();
	void Render(LPD3DXSPRITE pSprite);
	void Destroy();
	void AddLOG(string sLog);
};


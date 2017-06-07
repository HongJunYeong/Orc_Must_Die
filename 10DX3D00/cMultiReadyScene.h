#pragma once
#include "cScene.h"

class cMultiReadyScene :
	public cScene
{
public:
	cMultiReadyScene();
	virtual ~cMultiReadyScene();

private:
	enum
	{
		E_TEXT_1P_NAME = 10,
		E_TEXT_1P_READY,
		E_TEXT_2P_NAME,
		E_TEXT_2P_READY,
		E_TEXT_3P_NAME,
		E_TEXT_3P_READY,
		E_TEXT_4P_NAME,
		E_TEXT_4P_READY,
		E_TEXT_READY_COUNT,
	};

	cUIObject*			m_pUIRoot;
	cUIObject*			m_pCurrReadyBtn;
	cUIButton*			m_pReadyBtn;
	cUITextView*		m_pCountText;
	cUIButton*			m_pRelieveReadyBtn;

	cChatLog*			m_pLog;
	HANDLE				m_hRecvThread;
	DWORD				m_dwThreadID;
	ST_NETWORK			m_stNet;

	bool				m_isThreadClose;
	bool			    m_isAllReady;

	int					m_nCountStart;
	int					m_nCountEnd;
	int					m_nReadyCount;
public:
	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
	virtual void Release() override;

	//쓰레드
	virtual void StartRecvThread() override;

	//서버에서 데이터를 받을 함수
	void RecvNetwork();

	//On Click
	void OnClickReady();
	void OnClickRelieveReady();
	void OnClickBack();

	void Setup_UI();
	void RefreshRoom();
	void CheckReady();
	void ChangeCount();

	void LogUpdate(string sLog);
};


#pragma once
#include "cScene.h"

class cMenuScene : public cScene
{
public:
	cMenuScene();
	virtual ~cMenuScene();

private:
	cUIObject*			m_pUIRoot;
	cUIImageView*	    m_pBtnBgImage;
	vector<cUIButton*>	m_vecBtn;

	bool				m_isBtnOver;
public:
	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
	virtual void Release() override;

	void Setup_UI();

	void OnClickMutiPlay();
	void OnClickExit();
};


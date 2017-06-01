#pragma once
#include "cScene.h"

class cSetIPScene : public cScene
{
public:
	cSetIPScene();
	virtual ~cSetIPScene();

private:
	cUIObject*		m_pUIRoot;
	HWND			m_hNameEdit;
	HFONT			m_hFont;
public:
	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
	virtual void Release() override;

	void Setup_UI();

	void OnClickNextScene();
	void OnClickPrevScene();
};


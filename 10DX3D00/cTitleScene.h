#pragma once
#include "cScene.h"

class cTitleScene : public cScene
{
public:
	cTitleScene();
	virtual ~cTitleScene();

private:
	cUIObject*			m_pUIRoot;
	cUIImageView*		m_pUIPressAnyKeyImage;

	int					m_nPressAnykeyAlpha;

	bool				m_isPressKeyAlphaUp;
public:
	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
	virtual void Release() override;

	void Setup_UI();
	
};


#pragma once

class cUIObject;
class cUIImageView;
class cUIButton;
class cUITextView;
class cChatLog;

class cScene
{
public:
	cScene();
	virtual ~cScene();

protected:
	bool			m_isSceneChange;
	cUIObject*		m_pUIScreenChageImg;
	int				m_nScreenChangeAlpha;
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render(LPD3DXSPRITE pSprite);
	virtual void Release();
};


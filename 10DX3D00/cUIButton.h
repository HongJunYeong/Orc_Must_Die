#pragma once
#include "cUIObject.h"

class cUIButton;

class iButtonDelegate
{
public:
	virtual void OnClick(cUIButton* pSender) = 0;
};

class cUIButton : public cUIObject
{
public:
	cUIButton();
	virtual ~cUIButton();

	enum eButtonState
	{
		E_NORMAL,
		E_MOUSEOVER,
		E_SELECTED,
		E_STATE_CNT,
	};

	function<void()>	m_OnClick;
	bool				m_isScaleMode;

protected:

	eButtonState		m_eButtonState;
	LPDIRECT3DTEXTURE9	m_aTexture[E_STATE_CNT];

	// >>: 버튼 클릭에 대한 처리
	SYNTHESIZE(iButtonDelegate*, m_pDelegate, Delegate);
	// <<:

	RECT m_rc;
public:
	virtual void SetTexture(char* szNor, char* szOver, char* szSel);
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;

	inline void SetScaleMode(bool isScaleMode)
	{
		m_isScaleMode = isScaleMode;
	}

	inline D3DXVECTOR3& GetPos()
	{
		return m_vPosition;
	}
	inline eButtonState GetButtonState()
	{
		return m_eButtonState;
	}
	inline RECT& GetRect()
	{
		return m_rc;
	}
};


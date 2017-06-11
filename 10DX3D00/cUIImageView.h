#pragma once
#include "cUIObject.h"

class cUIImageView : public cUIObject
{
public:
	cUIImageView();
	virtual ~cUIImageView();

protected:
	LPDIRECT3DTEXTURE9	m_pTexture;
	bool				m_isRotZMode;

	float				m_fRotZ;
public:
	float				m_fCenterX;
	float				m_fCenterY;

	virtual void SetTexture(char* szFullPath);
	virtual void Render(LPD3DXSPRITE pSprite) override;
	
	float GetRotZ() { return m_fRotZ; }
	void SetRotZ(float z) { m_fRotZ = z; }

	void SetisRotZMode(bool isRotZMode) { m_isRotZMode = isRotZMode; }
};


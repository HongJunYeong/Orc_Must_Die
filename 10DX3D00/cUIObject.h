#pragma once
#include "cObject.h"

class cUIObject : public cObject
{
public:
	cUIObject();

public:
	virtual ~cUIObject();

protected:
	vector<cUIObject*>	m_vecChild;
	D3DXMATRIXA16		m_matWorld;
	int					m_nAlpha;
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(cUIObject*, m_pParent, Parent);
	SYNTHESIZE(ST_SIZEN, m_stSize, Size);

	bool m_isHidden;

	SYNTHESIZE(int, m_nTag, Tag);
public:

	virtual void SetPosition(float x, float y, float z = 0);
	virtual void AddChild(cUIObject* pChild);
	virtual void Update();
	virtual void Render(LPD3DXSPRITE pSprite);
	virtual void Destroy();
	virtual void SetAlpha(int nAlpha);
	virtual void SetisHidden(bool isHidden);
	virtual bool GetisHidden();
	virtual D3DXMATRIXA16& GetMatWorld();
	virtual cUIObject* FindChildByTag(int nTag);
};


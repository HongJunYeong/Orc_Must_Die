#include "stdafx.h"
#include "cUIImageView.h"


cUIImageView::cUIImageView()
	:m_pTexture(NULL)
{
}


cUIImageView::~cUIImageView()
{
}

void cUIImageView::SetTexture(char * szFullPath)
{
	D3DXIMAGE_INFO stImageInfo;
	m_pTexture = g_pTextureManager->GetTexture(szFullPath, &stImageInfo);

	m_stSize.nWidth = stImageInfo.Width;
	m_stSize.nHeight = stImageInfo.Height;

	m_fCenterX = 0.0f;
	m_fCenterY = 0.0f;
}

void cUIImageView::Render(LPD3DXSPRITE pSprite)
{
	if (m_isHidden) return;

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	if (m_isRotZMode)
	{
		D3DXMATRIXA16 matRZ;

		D3DXMatrixRotationZ(&matRZ, m_fRotZ);
		m_matWorld = matRZ * m_matWorld;
	}

	pSprite->SetTransform(&m_matWorld);

	RECT rc;
	SetRect(&rc, 0, 0, m_stSize.nWidth, m_stSize.nHeight);
	pSprite->Draw(
		m_pTexture,
		&rc,
		&D3DXVECTOR3(m_fCenterX, m_fCenterY, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(m_nAlpha, 255, 255, 255)
	);

	pSprite->End();

	cUIObject::Render(pSprite);
}

#include "stdafx.h"
#include "cCharacter.h"


cCharacter::cCharacter()
{
}


cCharacter::~cCharacter()
{
}

void cCharacter::Setup()
{
	D3DXCreateSphere(g_pD3DDevice,
		2.5f,
		10,
		10,
		&m_pHitColliderSpehre,
		NULL);

	m_vPostion = D3DXVECTOR3(-40.0f, 2.0f, -30.0f);
	m_hitColider.fRadius = 2.5f;
	m_hitColider.vCenter = m_vPostion;
}

void cCharacter::Update()
{
	if (GetKeyState('W') & 0x8000)
	{
		m_vPostion = m_vPostion + (m_vDirection * 0.2f);
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_vPostion = m_vPostion - (m_vDirection * 0.2f);
	}
	if (GetKeyState('A') & 0x8000)
	{
		m_fRotY -= 0.05f;
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_fRotY += 0.05f;
	}

	D3DXMATRIXA16 matT ,matR;

	D3DXMatrixRotationY(&matR, m_fRotY);
	D3DXMatrixTranslation(&matT, m_vPostion.x, m_vPostion.y, m_vPostion.z);

	m_hitColider.vCenter = m_vPostion;

	m_vDirection = D3DXVECTOR3(0, 0, 1.0f);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);

	m_matWorld = matR * matT;
}

void cCharacter::Render()
{
	g_pD3DDevice->SetTexture(0, 0);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pHitColliderSpehre->DrawSubset(0);
}

void cCharacter::Release()
{
	m_pHitColliderSpehre->Release();
}

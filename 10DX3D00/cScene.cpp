#include "stdafx.h"
#include "cScene.h"

#include "cUIImageView.h"

cScene::cScene()
{

}


cScene::~cScene()
{
}

void cScene::Setup()
{
	m_isSceneChange = true;
	m_nScreenChangeAlpha = 255;

	cUIImageView* pUIChangeImg = new cUIImageView;
	pUIChangeImg->SetAlpha(m_nScreenChangeAlpha);
	pUIChangeImg->SetPosition(0, 0);
	pUIChangeImg->SetTexture("Image/°ø¿ë/SceneChange.png");

	m_pUIScreenChageImg = pUIChangeImg;
}

void cScene::Update()
{
	if (m_isSceneChange)
	{
		m_nScreenChangeAlpha -= 5;
		m_pUIScreenChageImg->SetAlpha(m_nScreenChangeAlpha);
	}

	if (m_nScreenChangeAlpha <= 0)
	{
		m_nScreenChangeAlpha = 255;
		m_isSceneChange = false;
	}
}

void cScene::Render(LPD3DXSPRITE pSprite)
{
	m_pUIScreenChageImg->Render(pSprite);
}

void cScene::Release()
{
	m_pUIScreenChageImg->Destroy();
}

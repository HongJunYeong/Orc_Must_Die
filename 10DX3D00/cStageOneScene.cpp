#include "stdafx.h"
#include "cStageOneScene.h"

#include "cTile.h"

cStageOneScene::cStageOneScene()
{
}


cStageOneScene::~cStageOneScene()
{
}

void cStageOneScene::Setup()
{
}

void cStageOneScene::Update()
{
}

void cStageOneScene::Render(LPD3DXSPRITE pSprite)
{
	g_pGameManager->GetStageOneTile()->Render();
}

void cStageOneScene::Release()
{
}

void cStageOneScene::StartRecvThread()
{
}

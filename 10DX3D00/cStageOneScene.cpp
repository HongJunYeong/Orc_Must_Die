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
	g_pGameManager->StageOneUpdate();
}

void cStageOneScene::Render(LPD3DXSPRITE pSprite)
{
	g_pGameManager->StageOneRender();
}

void cStageOneScene::Release()
{
}

void cStageOneScene::StartRecvThread()
{
}

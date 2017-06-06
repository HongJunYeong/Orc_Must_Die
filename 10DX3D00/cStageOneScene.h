#pragma once
#include "cScene.h"

class cStageOneScene : public cScene
{
public:
	cStageOneScene();
	virtual ~cStageOneScene();

public:
	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
	virtual void Release() override;
	virtual void StartRecvThread() override;
};


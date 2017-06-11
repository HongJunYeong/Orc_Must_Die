#pragma once
#include "cScene.h"
class cLoadingScene :
	public cScene
{
public:
	cLoadingScene();
	virtual ~cLoadingScene();

private:
	HANDLE			m_hLoadThread;

	cUIImageView*	m_pLoadImage;
	cUIImageView*	m_pGearImage;

	float			m_fGearRotZ;

	bool			m_isLoadSuccess;
public:
	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
	virtual void Release() override;
	virtual void StartRecvThread() override;
	virtual void CloseThread() override;

	void Loading();
	void Setup_UI();
};


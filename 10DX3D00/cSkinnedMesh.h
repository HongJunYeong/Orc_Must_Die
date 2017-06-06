#pragma once
class cSkinnedMesh
{
public:
	cSkinnedMesh();
	~cSkinnedMesh();

protected:
	LPD3DXFRAME m_pRoot;
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;

	//	>>:

	float	m_fBlendTime;
	float	m_fPassedBlendTime;
	bool	m_isAnimBlend;
	//	<<:
public:
	void Setup(char* szFolder, char* szFile);
	void Update();
	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);
	void Render(LPD3DXFRAME pFrame, D3DXVECTOR3 s, D3DXVECTOR3 t);
	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame);
	void UpdateSkinnedMesh(LPD3DXFRAME pFrame);

	void SetAnimationIndex(int nIndex);
	void SetAnimationIndexBlend(int nIndex);

	/*
	m_pAnimController->GetNumAnimationSets();

	LPD3DXANIMATIONSET ...
	m_pAnimController->GetAnimationSet(index,.....);
	m_pAnimController->SetTrackAnimationSet(0,....);
	*/
};


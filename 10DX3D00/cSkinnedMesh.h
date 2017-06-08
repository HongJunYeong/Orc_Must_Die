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
	bool	m_bPlay;		//애니메이션이 플레이중인가??	
	bool	m_bLoop;		//애니메이션이 루프 플레이중인가??					
	//	<<:
public:
	void Setup(char* szFolder, char* szFile);
	void Update();
	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);
	void Render(LPD3DXFRAME pFrame, D3DXMATRIXA16* matWorld);
	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame);
	void UpdateSkinnedMesh(LPD3DXFRAME pFrame);

	void SetAnimationIndex(int nIndex);
	void SetAnimationIndexBlend(int nIndex);

	LPD3DXFRAME GetRootFrame();
	void SetRootFrame(LPD3DXFRAME pFrame);

	double GetAniTime(int nindex);//인덱스 위치의 애니메이션 길이를 가져옴
	double GetAniTrackPosition();//현재 진행중인 애니메이션의 위치를 가져옴

	//인덱스 위치의 애니메이션 종료시 선택한 애니메이션으로 변환
	//nIndex = 현재 사용중인 애니메이션 셋
	//nOutIndex = 현재 사용중인 애니메이션 종료시 변경하고자 하는 애니메이션 셋
	void SetNextAniMation(int nIndex, int nNextIndex);
};


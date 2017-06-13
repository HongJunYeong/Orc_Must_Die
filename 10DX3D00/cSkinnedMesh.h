#pragma once

struct ST_BONE;

class cSkinnedMesh
{
	//friend class cSkinnedMeshManager;

private:
	//하나만 생성
	ST_BONE*					m_pRootFrame;			
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;
	ST_SPHERE					m_stBoundingSphere;

	// 객체마다 생성
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	D3DXVECTOR3					m_vPosition;

	float m_fBlendTime;
	float m_fPassedBlendTime;
	bool  m_isAnimBlend;
public:
	cSkinnedMesh(char* szFolder, char* szFilename);
	cSkinnedMesh();
	~cSkinnedMesh(void);
	
	void UpdateAndRender(D3DXMATRIXA16& mat);
	void SetAnimationIndex(int nIndex);
	void SetAnimationIndexBlend(int nIndex);

	void SetRandomTrackPosition(); // 테스트용
	void SetPosition(D3DXVECTOR3 v)
	{
		m_vPosition = v;
		m_stBoundingSphere.vCenter = v;
	}
	ST_SPHERE* GetBoundingSphere()
	{
		return &m_stBoundingSphere;
	}

	ST_BONE* GetRootFrame();
	void SetRootFrame(ST_BONE* pFrame);

	double GetAniTime(int nIndex);//인덱스 위치의 애니메이션 길이를 가져옴
	double GetAniTrackPosition();//현재 진행중인 애니메이션의 위치를 가져옴

								 //인덱스 위치의 애니메이션 종료시 선택한 애니메이션으로 변환
								 //nIndex = 현재 사용중인 애니메이션 셋
								 //nOutIndex = 현재 사용중인 애니메이션 종료시 변경하고자 하는 애니메이션 셋
	bool SetNextAniMation(int nIndex, int nNextIndex);
	bool AnimationClose(int nIndex);

	void Load(char* szFolder, char* szFilename);
	LPD3DXEFFECT LoadEffect(char* szFilename);
	void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
	void Render(ST_BONE* pBone = NULL);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
	void Destroy();
private:
};


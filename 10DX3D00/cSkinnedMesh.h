#pragma once

struct ST_BONE;

class cSkinnedMesh
{
	//friend class cSkinnedMeshManager;

private:
	//�ϳ��� ����
	ST_BONE*					m_pRootFrame;			
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;
	ST_SPHERE					m_stBoundingSphere;

	// ��ü���� ����
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	D3DXVECTOR3					m_vPosition;

	D3DMATERIAL9				m_stMtl;

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

	void SetRandomTrackPosition(); // �׽�Ʈ��
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

	double GetAniTime(int nIndex);//�ε��� ��ġ�� �ִϸ��̼� ���̸� ������
	double GetAniTrackPosition();//���� �������� �ִϸ��̼��� ��ġ�� ������

								 //�ε��� ��ġ�� �ִϸ��̼� ����� ������ �ִϸ��̼����� ��ȯ
								 //nIndex = ���� ������� �ִϸ��̼� ��
								 //nOutIndex = ���� ������� �ִϸ��̼� ����� �����ϰ��� �ϴ� �ִϸ��̼� ��
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


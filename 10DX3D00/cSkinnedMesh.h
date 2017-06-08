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
	bool	m_bPlay;		//�ִϸ��̼��� �÷������ΰ�??	
	bool	m_bLoop;		//�ִϸ��̼��� ���� �÷������ΰ�??					
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

	double GetAniTime(int nindex);//�ε��� ��ġ�� �ִϸ��̼� ���̸� ������
	double GetAniTrackPosition();//���� �������� �ִϸ��̼��� ��ġ�� ������

	//�ε��� ��ġ�� �ִϸ��̼� ����� ������ �ִϸ��̼����� ��ȯ
	//nIndex = ���� ������� �ִϸ��̼� ��
	//nOutIndex = ���� ������� �ִϸ��̼� ����� �����ϰ��� �ϴ� �ִϸ��̼� ��
	bool SetNextAniMation(int nIndex, int nNextIndex);
};


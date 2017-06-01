#pragma once
#include "cObject.h"
#include "cMtlTex.h"

class cFrame : public cObject
{
public:
	cFrame();
	~cFrame();
private:
	SYNTHESIZE_PASS_BY_REF(vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matLocalTM,LocalTM);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorldTM, WorldTM);
	SYNTHESIZE_PASS_BY_REF(vector<ST_POS_SAMPLE>, m_vecPosTrack, PosTrack);
	SYNTHESIZE_PASS_BY_REF(vector<ST_ROT_SAMPLE>, m_vecRotTrack, RotTrack);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

	vector<cFrame*> m_vecChild;

	LPD3DXMESH	m_pMesh;
public:
	void Update(int nKeyFrame, D3DXMATRIXA16 * pMatParent);
	void Render();
	void MeshRender();
	void AddChild(cFrame* pChild);
	void Destroy();
	void ClacOrginalLocalTM(D3DXMATRIXA16* pmatParent);

	DWORD m_dwFirstFrame;
	DWORD m_dwLastFrame;
	DWORD m_dwFrameSpeed;
	DWORD m_dwTickSperFrame;

	int GetKeyFrame(int speed);
	void CalcLocalT(IN int nKeyFrame, OUT D3DXMATRIXA16& matT);
	void CalcLocalR(IN int nKeyFrame, OUT D3DXMATRIXA16& matR);

private:
	int							m_nNumTri;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;

public:
	void BuildVB(vector<ST_PNT_VERTEX>& vecVertex);
	void BuildMesh(vector<ST_PNT_VERTEX>& vecVertex);
};


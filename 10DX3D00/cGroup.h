#pragma once
#include "cObject.h"
#include "cMtlTex.h"

class cGroup : public cObject
{
public:
	cGroup();

	LPD3DXMESH					m_pMesh;
	IDirect3DVertexBuffer9*		m_pVB;
	IDirect3DIndexBuffer9*		m_pIB;
private:
	SYNTHESIZE_PASS_BY_REF(vector<ST_PNT_VERTEX>,
		m_vecVertex, Vertex);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

public:
	virtual ~cGroup();
	void Render();
};


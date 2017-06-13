#include "stdafx.h"
#include "cFrame.h"


cFrame::cFrame()
	:m_pMtlTex(NULL)
	,m_nNumTri(0)
	,m_pVB(NULL)
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
}


cFrame::~cFrame()
{
	SAFE_RELEASE(m_pMtlTex);
	SAFE_RELEASE(m_pVB);
}

void cFrame::Update(int nKeyFrame, D3DXMATRIXA16 * pMatParent)
{
	D3DXMATRIXA16 matR, matT;

	CalcLocalR(nKeyFrame, matR);
	CalcLocalT(nKeyFrame, matT);

	m_matLocalTM = matR *matT;
	m_matWorldTM = m_matLocalTM;

	if (pMatParent)
		m_matWorldTM *= (*pMatParent);
	

	for each(auto child in m_vecChild)
	{
		child->Update(nKeyFrame, &m_matWorldTM);
	}
}


void cFrame::Render()
{
	if (m_pMtlTex)
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
		g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMaterial());
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);


		/*g_pD3DDevice->DrawPrimitiveUP(
			D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_PNT_VERTEX)
		);*/


		////스트림소스 지정
		g_pD3DDevice->SetStreamSource(
			0,
			m_pVB,
			0,
			sizeof(ST_PNT_VERTEX)
		);


		g_pD3DDevice->DrawPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			m_nNumTri
		);

	}

	for each(auto child in m_vecChild)
	{
		child->Render();
	}
}

void cFrame::MeshRender()
{
	if (m_pMtlTex)
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
		g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMaterial());
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());

		
		m_pMesh->DrawSubset(0);
	}

	for each(auto child in m_vecChild)
	{
		child->MeshRender();
	}
}

void cFrame::AddChild(cFrame * pChild)
{
	m_vecChild.push_back(pChild);
}

void cFrame::Destroy()
{
	for each(auto child in m_vecChild)
	{
		child->Destroy();
	}

	SAFE_DELETE(m_pMtlTex);
	this->Release();
}

void cFrame::ClacOrginalLocalTM(D3DXMATRIXA16 * pmatParent)
{
	m_matLocalTM = m_matWorldTM;

	if (pmatParent)
	{
		D3DXMATRIXA16 matInvParent;
		D3DXMatrixInverse(&matInvParent, NULL, pmatParent);

		m_matLocalTM = m_matWorldTM * matInvParent;
	}

	for each(auto child in m_vecChild)
		child->ClacOrginalLocalTM(&m_matWorldTM);
}

int cFrame::GetKeyFrame(int speed)
{
	int first = m_dwFirstFrame * m_dwTickSperFrame;
	int last = m_dwLastFrame * m_dwTickSperFrame;
	int frameSpeed = m_dwFrameSpeed / 6;
	return (GetTickCount() * speed)% (last - first) + first;
}

void cFrame::CalcLocalT(IN int nKeyFrame, OUT D3DXMATRIXA16 & matT)
{
	D3DXMatrixIdentity(&matT);

	if (m_vecPosTrack.empty())
	{
		matT._41 = m_matLocalTM._41;
		matT._42 = m_matLocalTM._42;
		matT._43 = m_matLocalTM._43;
		return;
	}

	if (nKeyFrame <= m_vecPosTrack.front().n)
	{
		matT._41 = m_vecPosTrack.front().v.x;
		matT._42 = m_vecPosTrack.front().v.y;
		matT._43 = m_vecPosTrack.front().v.z;
		return;
	}

	if (nKeyFrame >= m_vecPosTrack.back().n)
	{
		matT._41 = m_vecPosTrack.back().v.x;
		matT._42 = m_vecPosTrack.back().v.y;
		matT._43 = m_vecPosTrack.back().v.z;
		return;
	}

	int nNextIndex = 0;

	for (size_t i = 0; i < m_vecPosTrack.size(); i++)
	{
		if (nKeyFrame < m_vecPosTrack[i].n)
		{
			nNextIndex = i;
			break;
		}
	}

	int nPrevIndex = nNextIndex - 1;

	float t = (nKeyFrame - m_vecPosTrack[nPrevIndex].n) /
		(float)(m_vecPosTrack[nNextIndex].n - m_vecPosTrack[nPrevIndex].n);

	D3DXVECTOR3 v;
	D3DXVec3Lerp(&v, &m_vecPosTrack[nPrevIndex].v, &m_vecPosTrack[nNextIndex].v, t);
	matT._41 = v.x;
	matT._42 = v.y;
	matT._43 = v.z;
}

void cFrame::CalcLocalR(IN int nKeyFrame, OUT D3DXMATRIXA16& matR)
{
	D3DXMatrixIdentity(&matR);

	if (m_vecRotTrack.empty())
	{
		matR = m_matLocalTM;
		matR._41 = 0.0f;
		matR._42 = 0.0f;
		matR._43 = 0.0f;
		return;
	}

	if (nKeyFrame <= m_vecRotTrack.front().n)
	{
		D3DXMatrixRotationQuaternion(&matR, &m_vecRotTrack.front().q);
		return;
	}

	if (nKeyFrame >= m_vecRotTrack.back().n)
	{
		D3DXMatrixRotationQuaternion(&matR, &m_vecRotTrack.back().q);
		return;
	}

	int nNextIndex = 0;

	for (size_t i = 0; i < m_vecRotTrack.size(); i++)
	{
		if (nKeyFrame < m_vecRotTrack[i].n)
		{
			nNextIndex = i;
			break;
		}
	}

	int nPrevIndex = nNextIndex - 1;

	D3DXQUATERNION q;
	float t = (nKeyFrame - m_vecRotTrack[nPrevIndex].n) /
		(float)(m_vecRotTrack[nNextIndex].n - m_vecRotTrack[nPrevIndex].n);
	D3DXQuaternionSlerp(&q, &m_vecRotTrack[nPrevIndex].q, &m_vecRotTrack[nNextIndex].q, t);
	D3DXMatrixRotationQuaternion(&matR, &q);
}

void cFrame::BuildVB(vector<ST_PNT_VERTEX>& vecVertex)
{
	m_nNumTri = vecVertex.size() / 3;

	g_pD3DDevice->CreateVertexBuffer(
		vecVertex.size() * sizeof(ST_PNT_VERTEX),
		0,
		ST_PNT_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL
	);

	ST_PNT_VERTEX* pV;

	m_pVB->Lock(0, 0, (LPVOID*)&pV, 0);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pVB->Unlock();
}

void cFrame::BuildMesh(vector<ST_PNT_VERTEX>& vecVertex)
{
	D3DXCreateMeshFVF(
		vecVertex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh
	);

	ST_PNT_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	WORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	for (int i = 0; i < vecVertex.size(); i++)
	{
		pI[i] = i;
	}
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	for (int i = 0; i < vecVertex.size() / 3; i++)
	{
		pA[i] = 0;
	}
	m_pMesh->UnlockAttributeBuffer();

	vector<DWORD> vecAdj(vecVertex.size());
	m_pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0],
		0, 0, 0
	);

}

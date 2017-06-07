#include "stdafx.h"
#include "cTile.h"
#include "cPyramid.h"

cTile::cTile()
{
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
}

cTile::~cTile()
{
	SAFE_RELEASE(m_pTexture);
}

void cTile::Setup(int nTileNum, float fInterval)
{
	for (int i = 0; i < nTileNum; ++i)
	{
		for (int j = 0; j < nTileNum; ++j)
		{
			ST_TILE_INFO  stTileInfo;
			ST_PNT_VERTEX vertex;

			vertex.p = D3DXVECTOR3(-(nTileNum / ((1.0f / fInterval) * 2.0f)) + i * fInterval,
									0.0f,
									-(nTileNum / ((1.0f / fInterval) * 2.0f)) + j * fInterval);
			vertex.t = D3DXVECTOR2(0, 1);
			m_vecVertex.push_back(vertex);
			stTileInfo.rc.left   = vertex.p.x;
			stTileInfo.rc.bottom = vertex.p.z;

			vertex.p = D3DXVECTOR3(-(nTileNum / ((1.0f / fInterval) * 2.0f)) + i * fInterval, 
									0.0f, 
									fInterval + -(nTileNum / ((1.0f / fInterval) * 2.0f)) + j * fInterval);
			vertex.t = D3DXVECTOR2(0, 0);
			m_vecVertex.push_back(vertex);

			vertex.p = D3DXVECTOR3(fInterval + -(nTileNum / ((1.0f / fInterval) * 2.0f)) + i * fInterval, 
									0.0f, 
									fInterval + -(nTileNum / ((1.0f / fInterval) * 2.0f)) + j * fInterval);
			vertex.t = D3DXVECTOR2(1, 0);
			m_vecVertex.push_back(vertex);
			stTileInfo.rc.right = vertex.p.x;
			stTileInfo.rc.top   = vertex.p.z;

			vertex.p = D3DXVECTOR3(-(nTileNum / ((1.0f / fInterval) * 2.0f)) + i * fInterval, 
									0.0f, 
									-(nTileNum / ((1.0f / fInterval) * 2.0f)) + j * fInterval);
			vertex.t = D3DXVECTOR2(0, 1);
			m_vecVertex.push_back(vertex);

			vertex.p = D3DXVECTOR3(fInterval + -(nTileNum / ((1.0f / fInterval) * 2.0f)) + i * fInterval, 
									0.0f, 
									fInterval + -(nTileNum / ((1.0f / fInterval) * 2.0f)) + j * fInterval);
			vertex.t = D3DXVECTOR2(1, 0);
			m_vecVertex.push_back(vertex);

			vertex.p = D3DXVECTOR3(fInterval + -(nTileNum / ((1.0f / fInterval) * 2.0f)) + i * fInterval, 
									0.0f, 
									-(nTileNum / ((1.0f / fInterval) * 2.0f)) + j * fInterval);
			vertex.t = D3DXVECTOR2(1, 1);
			m_vecVertex.push_back(vertex);

			stTileInfo.idX = i;
			stTileInfo.idY = j;
			
			stTileInfo.vecCenter = D3DXVECTOR3((stTileInfo.rc.left + stTileInfo.rc.right) / 2.0f,
												0.0f,
												(stTileInfo.rc.top + stTileInfo.rc.bottom) / 2.0f);

			m_stTileInfo.push_back(stTileInfo);
		}
	}

	D3DXVECTOR3 u, v, n;
	for (int i = 0; i < m_vecVertex.size(); i += 3)
	{
		u = m_vecVertex[i + 1].p - m_vecVertex[i].p;
		v = m_vecVertex[i + 2].p - m_vecVertex[i].p;

		D3DXVec3Cross(&n, &u, &v);
		D3DXVec3Normalize(&n, &n);

		m_vecVertex[i + 0].n = n;
		m_vecVertex[i + 1].n = n;
		m_vecVertex[i + 2].n = n;
	}
}

void cTile::Render()
{
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);

	g_pD3DDevice->SetTexture(0, m_pTexture);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof(ST_PNT_VERTEX));
	g_pD3DDevice->SetTexture(0, NULL);

	for each(auto p in m_vecPyramid)
	{
		p->Render();
	}
}

ST_TILE_INFO cTile::FindTile(ST_TILE_INFO::eTYPE type)
{

	return ST_TILE_INFO();
}

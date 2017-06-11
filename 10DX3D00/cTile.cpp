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
				(nTileNum / ((1.0f / fInterval) * 2.0f)) - j * fInterval);
			vertex.t = D3DXVECTOR2(0, 1);
			m_vecVertex.push_back(vertex);
			stTileInfo.rc.left = vertex.p.x;
			stTileInfo.rc.bottom = vertex.p.z;

			vertex.p = D3DXVECTOR3(-(nTileNum / ((1.0f / fInterval) * 2.0f)) + i * fInterval,
				0.0f,
				fInterval + (nTileNum / ((1.0f / fInterval) * 2.0f)) - j * fInterval);
			vertex.t = D3DXVECTOR2(0, 0);
			m_vecVertex.push_back(vertex);

			vertex.p = D3DXVECTOR3(fInterval + -(nTileNum / ((1.0f / fInterval) * 2.0f)) + i * fInterval,
				0.0f,
				fInterval + (nTileNum / ((1.0f / fInterval) * 2.0f)) - j * fInterval);
			vertex.t = D3DXVECTOR2(1, 0);
			m_vecVertex.push_back(vertex);
			stTileInfo.rc.right = vertex.p.x;
			stTileInfo.rc.top = vertex.p.z;

			vertex.p = D3DXVECTOR3(-(nTileNum / ((1.0f / fInterval) * 2.0f)) + i * fInterval,
				0.0f,
				(nTileNum / ((1.0f / fInterval) * 2.0f)) - j * fInterval);
			vertex.t = D3DXVECTOR2(0, 1);
			m_vecVertex.push_back(vertex);

			vertex.p = D3DXVECTOR3(fInterval + -(nTileNum / ((1.0f / fInterval) * 2.0f)) + i * fInterval,
				0.0f,
				fInterval + (nTileNum / ((1.0f / fInterval) * 2.0f)) - j * fInterval);
			vertex.t = D3DXVECTOR2(1, 0);
			m_vecVertex.push_back(vertex);

			vertex.p = D3DXVECTOR3(fInterval + -(nTileNum / ((1.0f / fInterval) * 2.0f)) + i * fInterval,
				0.0f,
				(nTileNum / ((1.0f / fInterval) * 2.0f)) - j * fInterval);
			vertex.t = D3DXVECTOR2(1, 1);
			m_vecVertex.push_back(vertex);

			stTileInfo.idX = j;
			stTileInfo.idY = i;

			stTileInfo.vecCenter = D3DXVECTOR3((stTileInfo.rc.left + stTileInfo.rc.right) / 2.0f,
				0.0f,
				(stTileInfo.rc.top + stTileInfo.rc.bottom) / 2.0f);

			m_stTileInfo.push_back(stTileInfo);
		}
	}

	{
		FILE *fp;
		fopen_s(&fp, "TileInfo.txt", "r");

		for (int j = 0; j < m_stTileInfo.size(); ++j)
		{
			int nt = m_stTileInfo.size();

			char szTemp[10];
			fgets(szTemp, 10, fp);

			int n;
			sscanf_s(szTemp, "%d", &n);

			m_stTileInfo[j].type = (ST_TILE_INFO::eTYPE)n;

		}
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

	g_pD3DDevice->SetTexture(0, 0);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof(ST_PNT_VERTEX));
	g_pD3DDevice->SetTexture(0, NULL);
}

int cTile::FindArrForXZ(float x, float z)
{
	int nCellsPerCol = NUM_TILE - 1;
	int nCellsPerRow = NUM_TILE - 1;
	x = ((float)nCellsPerCol / 2.0f) + x;
	z = ((float)nCellsPerRow / 2.0f) - z;

	x /= 1.0f;
	z /= 1.0f;

	int col = floorf(x);
	int row = floorf(z);

	return col * NUM_TILE + row + 1;
}

int cTile::FindArrForTileType(ST_TILE_INFO::eTYPE type)
{
	for (int i = 0; i < m_stTileInfo.size(); i++)
	{
		if(type == m_stTileInfo[i].type)
			return i;
	}
}

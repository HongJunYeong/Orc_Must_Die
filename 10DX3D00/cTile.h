#pragma once

class cPyramid; 

class cTile
{
public:
	cTile();
	~cTile();

private:
	vector<ST_PNT_VERTEX>	m_vecVertex;
	D3DMATERIAL9			m_stMtl;
	LPDIRECT3DTEXTURE9		m_pTexture;
	LPD3DXMESH				m_pMesh;

	vector<ST_TILE_INFO>	m_stTileInfo;
	std::vector<cPyramid*>	m_vecPyramid;

public:
	void Setup(int nTileNum, float fInterval, char* szFullPath);
	void Render();
	vector<ST_PNT_VERTEX>& GetVertex() { return m_vecVertex; }
	vector<ST_TILE_INFO>& GetTileInfo() { return m_stTileInfo; }
	vector<ST_TILE_INFO> GetTileInfoValue() { return m_stTileInfo; }
	int FindArr(int x, int y) { return x * NUM_TILE + y; }
	void FindArr(int arr, int& x, int& y) { x = arr / NUM_TILE, y = arr % NUM_TILE; }
	ST_TILE_INFO FindTile(ST_TILE_INFO::eTYPE type);
};
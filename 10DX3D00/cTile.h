#pragma once

class cTile
{
public:
	cTile();
	~cTile();

private:
	vector<ST_PNT_VERTEX>	m_vecVertex;
	D3DMATERIAL9			m_stMtl;

	vector<ST_TILE_INFO>	m_stTileInfo;
public:
	void Setup(int nTileNum, float fInterval);
	void Render();
	vector<ST_PNT_VERTEX>& GetVertex() { return m_vecVertex; }
	vector<ST_TILE_INFO>& GetTileInfo() { return m_stTileInfo; }
	vector<ST_TILE_INFO> GetTileInfoValue() { return m_stTileInfo; }
	int FindArr(int x, int y) { return y * NUM_TILE + x; }
	void FindArr(int arr, int& x, int& y) { y = arr / NUM_TILE, x = arr % NUM_TILE; }
	int FindArrForXZ(float x, float z);
	ST_TILE_INFO FindTile(ST_TILE_INFO::eTYPE type);
};
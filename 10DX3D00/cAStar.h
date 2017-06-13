#pragma once

class cTile;

#define TILE_X 300
#define TILE_Z 300

class cAStar
{
public:
	cAStar();
	~cAStar();


private:
	enum {
		E_LEFT = 1 << 0,
		E_RIGHT = 1 << 1,
		E_UP = 1 << 2,
		E_DOWN = 1 << 3
	};

	set<ST_TILE_INFO*>		m_setOpenList;
	set<ST_TILE_INFO*>		m_setCloseList;
	vector<int>				m_vecDirection;
	map<int, float>			m_mapDistance;

	D3DMATERIAL9			m_stMtlRed;
	D3DMATERIAL9			m_stMtlGreen;
	D3DMATERIAL9			m_stMtlBlue;
	D3DMATERIAL9			m_stMtlYellow;
	D3DMATERIAL9			m_stMtlBlack;
	D3DMATERIAL9			m_stMtlPurple;

	ST_TILE_INFO*			m_pTileStart;
	ST_TILE_INFO*			m_pTileEnd;

public:
	vector<ST_TILE_INFO>	m_vecTile;
	vector<D3DXVECTOR3>     m_vecDest;

	void Setup();
	void Update();
	void Render();

	vector<D3DXVECTOR3> FindPath(vector<ST_TILE_INFO> vecTile);
	ST_TILE_INFO* FindTile(ST_TILE_INFO::eAStarType e);
	float CalcHeuristic(ST_TILE_INFO* tile1, ST_TILE_INFO* tile2);
	ST_TILE_INFO* FindMinFNodeAtOpenList();
	void MarkNodeType(ST_TILE_INFO* startTile, ST_TILE_INFO* endTile);
	bool IsInList(ST_TILE_INFO* tile, set<ST_TILE_INFO*> list);
	void Extend(ST_TILE_INFO* centerTile, ST_TILE_INFO* endTile);
	ST_TILE_INFO* GetAdjNode(ST_TILE_INFO* centerTile, int dir);
};


#include "stdafx.h"
#include "cAStar.h"

#include "cTile.h"

cAStar::cAStar()
{
}


cAStar::~cAStar()
{
}

void cAStar::Setup()
{
	m_vecDirection.push_back(E_LEFT);
	m_vecDirection.push_back(E_RIGHT);
	m_vecDirection.push_back(E_UP);
	m_vecDirection.push_back(E_DOWN);
	m_vecDirection.push_back(E_LEFT | E_UP);
	m_vecDirection.push_back(E_LEFT | E_DOWN);
	m_vecDirection.push_back(E_RIGHT | E_UP);
	m_vecDirection.push_back(E_RIGHT | E_DOWN);

	float fSqrt = sqrt(2.0f);
	m_mapDistance[E_LEFT] = 1.0f;
	m_mapDistance[E_RIGHT] = 1.0f;
	m_mapDistance[E_UP] = 1.0f;
	m_mapDistance[E_DOWN] = 1.0f;
	m_mapDistance[E_LEFT | E_UP] = fSqrt;
	m_mapDistance[E_LEFT | E_DOWN] = fSqrt;
	m_mapDistance[E_RIGHT | E_UP] = fSqrt;
	m_mapDistance[E_RIGHT | E_DOWN] = fSqrt;

}

void cAStar::Update()
{
}

void cAStar::Render()
{
	/*for (int i = 0; i < m_vecTile.size(); i++)
	{
		m_vecTile[i].Render();
	}*/
}

vector<D3DXVECTOR3> cAStar::FindPath(ST_TILE_INFO* Start, ST_TILE_INFO* End, vector<ST_TILE_INFO> vecTile)
{
	m_setOpenList.clear();
	m_setCloseList.clear();
	m_vecDest.clear();
	m_vecTile.clear();

	m_vecTile = vecTile;

	//cTile* pStartTile = FindTile(cTile::E_START);
	//cTile* pEndTile = FindTile(cTile::E_END);

	ST_TILE_INFO* pStartTile = Start;
	ST_TILE_INFO* pEndTile = End;

	pStartTile->fG = 0.0f;
	pStartTile->fH = CalcHeuristic(pStartTile, pEndTile);
	pStartTile->fF = pStartTile->fG + pStartTile->fH;

	m_setOpenList.insert(pStartTile);

	while (true)
	{
		ST_TILE_INFO* pMinFNode = FindMinFNodeAtOpenList();

		if (pMinFNode == NULL)
		{
			MarkNodeType(pStartTile, pEndTile);
			break;
		}
		
		m_setOpenList.erase(pMinFNode);
		m_setCloseList.insert(pMinFNode);

		
		Extend(pMinFNode, pEndTile);

		if (pMinFNode == pEndTile)
		{
			MarkNodeType(pStartTile, pEndTile);
			break;
		}
	}

	return m_vecDest;
	
}

//cTile * cAStar::FindTile(cTile::eTILE_TYPE e)
//{
//	for (int i = 0; i < m_vecTile.size(); i++)
//	{
//		if (m_vecTile[i]->GetTileType() == e)
//			return m_vecTile[i];
//	}
//}

float cAStar::CalcHeuristic(ST_TILE_INFO* tile1, ST_TILE_INFO* tile2)
{
	return fabs((float)(tile1->idX - tile2->idX))
		+ fabs((float)(tile1->idY - tile2->idY));
}

ST_TILE_INFO* cAStar::FindMinFNodeAtOpenList()
{
	float fMinF = FLT_MAX;
	ST_TILE_INFO* pMinTile = NULL;

	set<ST_TILE_INFO*>::iterator iter;
	for (iter = m_setOpenList.begin(); iter != m_setOpenList.end(); ++iter)
	{
		if ((*iter)->fF < fMinF)
		{
			pMinTile = (*iter);
			fMinF = (*iter)->fF;
		}
	}
	return pMinTile;
}

void cAStar::MarkNodeType(ST_TILE_INFO* startTile, ST_TILE_INFO* endTile)
{
	for (int i=0 ; i < m_vecTile.size(); i++)
	{
		if (IsInList(&m_vecTile[i], m_setOpenList))
		{
			m_vecTile[i].aStarType = ST_TILE_INFO::OPEN;
		}
		if (IsInList(&m_vecTile[i], m_setCloseList))
		{
			m_vecTile[i].aStarType = ST_TILE_INFO::CLOSE;
		}
	}

	ST_TILE_INFO* pTemp = endTile;
	while (pTemp)
	{
		if (pTemp == startTile)
		{
			pTemp->aStarType = ST_TILE_INFO::START;
		}
		else if (pTemp == endTile)
		{
			pTemp->aStarType = ST_TILE_INFO::END;
		}
		else
		{
			pTemp->aStarType = ST_TILE_INFO::PATH;
			m_vecDest.push_back(D3DXVECTOR3(pTemp->vecCenter[0],0, pTemp->vecCenter[2]));
		}
		pTemp = pTemp->pParent;
	}
}

bool cAStar::IsInList(ST_TILE_INFO*  tile, set<ST_TILE_INFO*> list)
{
	return list.find(tile) != list.end();
}

void cAStar::Extend(ST_TILE_INFO*  centerTile, ST_TILE_INFO* endTile)
{
	for (int i=0; i < m_vecDirection.size(); i++)
	{
		ST_TILE_INFO* pAdjNode = GetAdjNode(centerTile, m_vecDirection[i]);

		if (pAdjNode == NULL) continue;

		if (IsInList(pAdjNode, m_setOpenList))
		{
			if (pAdjNode->fG > centerTile->fG + m_mapDistance[m_vecDirection[i]])
			{
				pAdjNode->pParent = centerTile;
				pAdjNode->fG = centerTile->fG + m_mapDistance[m_vecDirection[i]];
				pAdjNode->fF = pAdjNode->fG + pAdjNode->fH;
			}
		}
		else
		{
			pAdjNode->pParent = centerTile;
			pAdjNode->fG = centerTile->fG + m_mapDistance[m_vecDirection[i]];
			pAdjNode->fH = CalcHeuristic(pAdjNode, endTile);
			pAdjNode->fF = pAdjNode->fG + pAdjNode->fH;
			m_setOpenList.insert(pAdjNode);
		}
	}
}

ST_TILE_INFO* cAStar::GetAdjNode(ST_TILE_INFO* centerTile, int dir)
{
	int nAdjRow = centerTile->idX;
	int nAdjCol = centerTile->idY;

	if (dir & E_LEFT)
	{
		if (centerTile->idX == 0) return NULL;

		int nIndex = nAdjCol * TILE_X + nAdjRow - 1;

		if (m_vecTile[nIndex].type == ST_TILE_INFO::WALL) return NULL;
		if (m_vecTile[nIndex].type == ST_TILE_INFO::MONSTER) return NULL;
		nAdjRow -= 1;
	}
	if (dir & E_RIGHT)
	{
		if (centerTile->idX == TILE_X - 1) return NULL;
		int nIndex = nAdjCol * TILE_X + nAdjRow + 1;
		if (m_vecTile[nIndex].type == ST_TILE_INFO::WALL) return NULL;
		if (m_vecTile[nIndex].type == ST_TILE_INFO::MONSTER) return NULL;
		nAdjRow += 1;
	}
	if (dir & E_UP)
	{
		if (centerTile->idY == 0) return NULL;
		int nIndex = (nAdjCol - 1)* TILE_X + nAdjRow;
		if (m_vecTile[nIndex].type == ST_TILE_INFO::WALL) return NULL;
		if (m_vecTile[nIndex].type == ST_TILE_INFO::MONSTER) return NULL;
		nAdjCol -= 1;
	}
	if (dir & E_DOWN)
	{
		if (centerTile->idY == TILE_X - 1) return NULL;
		int nIndex = (nAdjCol + 1)* TILE_X + nAdjRow;
		if (m_vecTile[nIndex].type == ST_TILE_INFO::WALL) return NULL;
		if (m_vecTile[nIndex].type == ST_TILE_INFO::MONSTER) return NULL;
		nAdjCol += 1;
	}

	int nIndex = nAdjCol * TILE_X + nAdjRow;
	if (m_vecTile[nIndex].type == ST_TILE_INFO::WALL) return NULL;
	if (m_vecTile[nIndex].type == ST_TILE_INFO::MONSTER) return NULL;
	if (IsInList(&m_vecTile[nIndex], m_setCloseList)) return NULL;

	return &m_vecTile[nIndex];
}

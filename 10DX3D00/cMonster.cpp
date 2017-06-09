#include "stdafx.h"
#include "cMonster.h"

#include "cSkinnedMesh.h"
#include "cAStar.h"
#include "cTile.h"

unsigned int __stdcall FindPathThread(LPVOID p);

cMonster::cMonster()
{
}


cMonster::~cMonster()
{
}

void cMonster::Setup()
{
	m_pSkinnedMesh = NULL;
	m_hPathThread = NULL;
	m_pAStar = new cAStar;
	m_pAStar->Setup();
	m_eMonsterState = E_IDLE;

	m_vPosition = D3DXVECTOR3(0, 0, 0);
	m_vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_vDirection = D3DXVECTOR3(0, 0, 1.0f);
	
	m_fRotY = 0.0f;
	m_isDie = false;
	m_isFindPath = false;
	m_isCollision = false;

	m_nAnimIndex = 0;

	int end = g_pGameManager->GetStageOneTile()->FindArrForTileType(ST_TILE_INFO::DEST);
	m_stEndTile = g_pGameManager->GetStageOneTile()->GetTileInfoValue()[end];

	m_hMutex = m_hMutex = CreateMutex(NULL, FALSE, NULL);
}

void cMonster::Update()
{
	srand(time(NULL));
}

void cMonster::Render()
{
}

void cMonster::Sphere_Render()
{
	D3DXMATRIXA16 matWorld, matT;
	D3DXMatrixIdentity(&matWorld);

	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y + 1.0f, m_vPosition.z);

	matWorld = matT;

	m_stTraceSphere.vCenter.x = matWorld._41;
	m_stTraceSphere.vCenter.y = matWorld._42;
	m_stTraceSphere.vCenter.z = matWorld._43;

	m_stAttackSphere.vCenter.x = matWorld._41;
	m_stAttackSphere.vCenter.y = matWorld._42;
	m_stAttackSphere.vCenter.z = matWorld._43;

	m_stHitSphere.vCenter.x = matWorld._41;
	m_stHitSphere.vCenter.y = matWorld._42;
	m_stHitSphere.vCenter.z = matWorld._43;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pAttackSphere->DrawSubset(0);
	m_pTraceSphere->DrawSubset(0);
	m_pHitSphere->DrawSubset(0);



	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void cMonster::RWeaponSphere_Render(string name)
{
	ST_BONE* m_pBone = (ST_BONE*)D3DXFrameFind(m_pSkinnedMesh->GetRootFrame(), name.c_str());
	D3DXMATRIXA16 matWorld, matS, matT;
	D3DXMatrixIdentity(&matWorld);

	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);

	matWorld = m_pBone->CombinedTransformationMatrix * matS * matT;

	m_stRWeaponSphere.vCenter.x = matWorld._41;
	m_stRWeaponSphere.vCenter.y = matWorld._42;
	m_stRWeaponSphere.vCenter.z = matWorld._43;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pRWeaponSphere->DrawSubset(0);

	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void cMonster::LWeaponSphere_Render(string name)
{
	ST_BONE* m_pBone = (ST_BONE*)D3DXFrameFind(m_pSkinnedMesh->GetRootFrame(), name.c_str());
	D3DXMATRIXA16 matWorld, matS, matT;
	D3DXMatrixIdentity(&matWorld);

	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	m_stLWeaponSphere.vCenter = D3DXVECTOR3(m_vPosition.x, m_vPosition.y, m_vPosition.z);

	matWorld = m_pBone->CombinedTransformationMatrix * matS * matT;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pLWeaponSphere->DrawSubset(0);

	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

float cMonster::Distance_Between_Two_Points(D3DXVECTOR3 v1, D3DXVECTOR3 v2)
{
	float dDistance = 0.0f;
	float x = pow(v1.x - v2.x, 2);
	float y = pow(v1.y - v2.y, 2);
	float z = pow(v1.z - v2.z, 2);

	return dDistance = sqrt(x + y + z);

}

void cMonster::Release()
{
	if (TerminateThread(m_hPathThread, 0))
	{
		CloseHandle(m_hPathThread);
	}

	SAFE_DELETE(m_pSkinnedMesh);
	SAFE_DELETE(m_pAStar);
	CloseHandle(m_hMutex);

	SAFE_DELETE(m_pSkinnedMesh);
	SAFE_RELEASE(m_pAttackSphere);
	SAFE_RELEASE(m_pTraceSphere);
	SAFE_RELEASE(m_pLWeaponSphere);
	SAFE_RELEASE(m_pRWeaponSphere);
	SAFE_RELEASE(m_pHitSphere);
}

void cMonster::StartThread()
{
	m_hPathThread = (HANDLE)_beginthreadex(NULL, 0, FindPathThread, this, CREATE_SUSPENDED, NULL);
}

void cMonster::FindPath()
{
	int FindPathStartTime = GetTickCount();
	int FindPathEndTime = GetTickCount() + 1000;

	while (true)
	{
		WaitForSingleObject(g_pGameManager->m_hSem, INFINITE);
		if (m_isDie) break;
		if (m_isFindPath) continue;

		FindPathStartTime = GetTickCount();

		if (FindPathStartTime >= FindPathEndTime)
		{
			FindPathEndTime = FindPathStartTime + 1000;
			m_vecDest.clear();

			vector<ST_TILE_INFO> vecTile = g_pGameManager->GetStageOneTile()->GetTileInfoValue();

			int start = g_pGameManager->GetStageOneTile()->FindArrForXZ(m_vPosition.x, m_vPosition.z);
			int end = g_pGameManager->GetStageOneTile()->FindArr(m_stEndTile.idX, m_stEndTile.idY);

			vecTile[start].aStarType = ST_TILE_INFO::START;
			vecTile[end].aStarType = ST_TILE_INFO::END;

			m_vecDest = m_pAStar->FindPath(vecTile);

			cout << m_vecDest.size() << endl;

			int n = m_vecDest.size();

			//
			m_isFindPath = true;

			m_vDirection = m_vecDest.back() - m_vPosition;
			D3DXVec3Normalize(&m_vDirection, &m_vDirection);

			D3DXVECTOR3 zAxis(0, 0, 1);
			m_fRotY = acosf(D3DXVec3Dot(&m_vDirection, &zAxis));
			if (m_vDirection.x >= 0) m_fRotY += D3DX_PI;
			else if (m_vDirection.x < 0) m_fRotY = -m_fRotY + D3DX_PI;
			//
		}
		ReleaseSemaphore(g_pGameManager->m_hSem,1,NULL);
	}
}

void cMonster::FindPath2()
{
	m_vecDest.clear();

	vector<ST_TILE_INFO> vecTile = g_pGameManager->GetStageOneTile()->GetTileInfoValue();

	int start = g_pGameManager->GetStageOneTile()->FindArrForXZ(m_vPosition.x, m_vPosition.z);
	int end = g_pGameManager->GetStageOneTile()->FindArr(m_stEndTile.idX, m_stEndTile.idY);

	vecTile[start].aStarType = ST_TILE_INFO::START;
	vecTile[end].aStarType = ST_TILE_INFO::END;

	m_vecDest = m_pAStar->FindPath(vecTile);

	cout << m_vecDest.size() << endl;

	int n = m_vecDest.size();

	//
	m_isFindPath = true;

	m_vDirection = m_vecDest.back() - m_vPosition;
	D3DXVec3Normalize(&m_vDirection, &m_vDirection);

	D3DXVECTOR3 zAxis(0, 0, 1);
	m_fRotY = acosf(D3DXVec3Dot(&m_vDirection, &zAxis));
	if (m_vDirection.x >= 0) m_fRotY += D3DX_PI;
	else if (m_vDirection.x < 0) m_fRotY = -m_fRotY + D3DX_PI;

	StopThread();
	//
}

void cMonster::ThreadResume()
{
	ResumeThread(m_hPathThread);
}

void cMonster::StopThread()
{
	SuspendThread(m_hPathThread);
}

unsigned int __stdcall FindPathThread(LPVOID p)
{
	cMonster* g = (cMonster*)p;
	g->FindPath();

	return 0;
}

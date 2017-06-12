#include "stdafx.h"
#include "cMonster.h"

#include "cSkinnedMesh.h"
#include "cAStar.h"
#include "cTile.h"
#include "cCharacter.h"

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
	m_isGameStart = true;

	m_isAttack = false;
	m_isTrace = false;
	m_isTraceForFinalDest = false;

	m_nAnimIndex = 0;

	int end = g_pGameManager->GetStageOneTile()->FindArrForTileType(ST_TILE_INFO::DEST);
	m_stEndTile = g_pGameManager->GetStageOneTile()->GetTileInfoValue()[end];

	m_hMutex = m_hMutex = CreateMutex(NULL, FALSE, NULL);
}

void cMonster::Update()
{

}

void cMonster::Render()
{
}

void cMonster::Move()
{
	if (m_isAttack) return;

	if (m_vecDest.size() > 0)
	{
		m_vPosition = m_vPosition + (m_vDirection * 0.1f);
		D3DXVECTOR3 v = m_vecDest.back() - m_vPosition;
		float len = D3DXVec3Length(&v);

		if (len <= 0.1f)
		{

			m_vPosition = m_vecDest.back();

			//int tileNum = g_pGameManager->GetStageOneTile()->FindArrForXZ(m_vecDest.back().x, m_vecDest.back().z);
			//g_pGameManager->GetStageOneTile()->GetTileInfo()[tileNum].type = ST_TILE_INFO::NONE;

			if (m_vPosition == m_stFinalDestTile.vecCenter)
			{
				m_isDie = true;
				return;
			}

			m_vecDest.pop_back();

			//tileNum = g_pGameManager->GetStageOneTile()->FindArrForXZ(m_vecDest.back().x, m_vecDest.back().z);
			//g_pGameManager->GetStageOneTile()->GetTileInfo()[tileNum].type = ST_TILE_INFO::MONSTER;

			if (m_vecDest.size() == 0)
			{
				//m_isFindPath = false;
				return;
			}
			m_vDirection = m_vecDest.back() - m_vPosition;
			D3DXVec3Normalize(&m_vDirection, &m_vDirection);

			D3DXVECTOR3 zAxis(0, 0, 1);
			m_fRotY = acosf(D3DXVec3Dot(&m_vDirection, &zAxis));
			if (m_vDirection.x >= 0) m_fRotY += D3DX_PI;
			else if (m_vDirection.x < 0) m_fRotY = -m_fRotY + D3DX_PI;
		}

		m_vDirection = m_vecDest.back() - m_vPosition;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
	}
}

void cMonster::CheckTraceTime()
{
	if (m_isAttack) return;

	m_nStartTime = GetTickCount();

	if (m_nStartTime >= m_nEndTime)
	{
		m_eMonsterState = E_NONE;
		m_isTrace = false;
		m_isTraceForFinalDest = true;
	}
}

void cMonster::TraceSeach()
{
	if (m_isAttack) return;

	for (int i = 0; i < g_pGameManager->GetCharacter().size(); i++)
	{
		float dist = Distance_Between_Three_Points(g_pGameManager->GetCharacter()[i]->GetHitCollider().vCenter,
			m_stTraceSphere.vCenter);

		if (dist <= (m_stTraceSphere.fRadius
			+ g_pGameManager->GetCharacter()[i]->GetHitCollider().fRadius))
		{
			int end = g_pGameManager->GetStageOneTile()->FindArrForXZ(
				g_pGameManager->GetCharacter()[i]->GetHitCollider().vCenter.x,
				g_pGameManager->GetCharacter()[i]->GetHitCollider().vCenter.z
			);

			ThreadResume();
			m_isFindPath = false;
			m_isTrace = true;
			m_stEndTile = g_pGameManager->GetStageOneTile()->GetTileInfoValue()[end];

			m_nStartTime = GetTickCount();
			m_nEndTime = GetTickCount() + 1000;

			return;
		}
	}

	if (m_isTraceForFinalDest)
	{
		ThreadResume();
		m_isFindPath = false;
		m_isTraceForFinalDest = false;
		m_stEndTile = m_stFinalDestTile;
	}
}

bool cMonster::AttackSearch()
{
	if (!m_isAttack)
	{
		for (int i = 0; i < g_pGameManager->GetCharacter().size(); i++)
		{
			float dist = Distance_Between_Three_Points(g_pGameManager->GetCharacter()[i]->GetHitCollider().vCenter,
				m_stHitSphere.vCenter);

			//두 점 사이의 거리 공식을 통해 몬스터 피격 범위와 캐릭터의 피격 범위가 충돌되었는지 파악
			if (dist <= (m_stHitSphere.fRadius
				+ g_pGameManager->GetCharacter()[i]->GetHitCollider().fRadius))
			{
				m_eMonsterState = E_ATTACK;
				m_isAttack = true;
				m_pSkinnedMesh->SetAnimationIndexBlend(1);

				//몬스터 무기 범위와 캐릭터 피격 범위 사이의 거리를 구함
				dist = Distance_Between_Three_Points(g_pGameManager->GetCharacter()[i]->GetHitCollider().vCenter,
					m_stRWeaponSphere.vCenter);

				//몬스터 무기 범위와 캐릭터 피격 범위가 충돌되었는지 파악
				if (dist <= (m_stRWeaponSphere.fRadius
					+ g_pGameManager->GetCharacter()[i]->GetHitCollider().fRadius))
				{
					//cout << "충돌됬으연" << endl;
					return true;
				}
			}
		}
	}

	return false;
}

void cMonster::Sphere_Render()
{
	D3DXMATRIXA16 matWorld, matT;
	D3DXMatrixIdentity(&matWorld);

	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

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
	//m_pAttackSphere->DrawSubset(0);
	//m_pTraceSphere->DrawSubset(0);
	//m_pHitSphere->DrawSubset(0);



	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void cMonster::RWeaponSphere_Render(string name)
{
	ST_BONE* m_pBone = (ST_BONE*)D3DXFrameFind(m_pSkinnedMesh->GetRootFrame(), name.c_str());

	D3DXMATRIXA16 matWorld, matS, matT,matR;
	D3DXMatrixIdentity(&matWorld);

	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationY(&matR, m_fRotY);

	matWorld = m_pBone->CombinedTransformationMatrix;

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
	D3DXMATRIXA16 matWorld, matS, matT, matR;
	D3DXMatrixIdentity(&matWorld);

	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationY(&matR, m_fRotY);

	m_stLWeaponSphere.vCenter.x = matWorld._41;
	m_stLWeaponSphere.vCenter.y = matWorld._42;
	m_stLWeaponSphere.vCenter.z = matWorld._43;

	matWorld = m_pBone->CombinedTransformationMatrix * matS * matT;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	//m_pLWeaponSphere->DrawSubset(0);

	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

float cMonster::Distance_Between_Three_Points(D3DXVECTOR3 v1, D3DXVECTOR3 v2)
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
	while (true)
	{

		if (m_isDie) break;
		if (m_isFindPath) continue;

		WaitForSingleObject(g_pGameManager->m_hSem, INFINITE);

		m_vecDest.clear();

		vector<ST_TILE_INFO> vecTile = g_pGameManager->GetStageOneTile()->GetTileInfoValue();

		int start = g_pGameManager->GetStageOneTile()->FindArrForXZ(m_vPosition.x, m_vPosition.z);
		int end = g_pGameManager->GetStageOneTile()->FindArr(m_stEndTile.idX, m_stEndTile.idY);

		vecTile[start].aStarType = ST_TILE_INFO::START;
		vecTile[end].aStarType = ST_TILE_INFO::END;

		m_vecDest = m_pAStar->FindPath(vecTile);

		m_isFindPath = true;
		m_eMonsterState = E_NONE;

		m_vDirection = m_vecDest.back() - m_vPosition;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);

		D3DXVECTOR3 zAxis(0, 0, 1);
		m_fRotY = acosf(D3DXVec3Dot(&m_vDirection, &zAxis));
		if (m_vDirection.x >= 0) m_fRotY += D3DX_PI;
		else if (m_vDirection.x < 0) m_fRotY = -m_fRotY + D3DX_PI;

		ReleaseSemaphore(g_pGameManager->m_hSem, 1, NULL);
		StopThread();
	}
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

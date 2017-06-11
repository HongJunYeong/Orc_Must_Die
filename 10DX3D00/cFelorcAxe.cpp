#include "stdafx.h"
#include "cFelorcAxe.h"

#include "cSkinnedMesh.h"
#include "cTile.h"

cFelorcAxe::cFelorcAxe()
{
}


cFelorcAxe::~cFelorcAxe()
{
}

void cFelorcAxe::Setup()
{
	cMonster::Setup();

	m_vScale = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
	m_vPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//공격 범위 설정
	m_stAttackSphere.fRadius = 2.0f;
	//탐지 범위 설정
	m_stTraceSphere.fRadius = 5.0f;
	//오른쪽 무기 범위 설정
	m_stRWeaponSphere.fRadius = 0.5f;
	//피격 범위 설정
	m_stHitSphere.fRadius = 1.2f;

	//공격 범위 표시용 원 설정
	D3DXCreateSphere(g_pD3DDevice, m_stAttackSphere.fRadius, 10, 10, &m_pAttackSphere, NULL);
	//탐지 범위 표시용 원 설정
	D3DXCreateSphere(g_pD3DDevice, m_stTraceSphere.fRadius, 10, 10, &m_pTraceSphere, NULL);
	//오른쪽 무기 범위 표시용 원 설정
	D3DXCreateSphere(g_pD3DDevice, m_stRWeaponSphere.fRadius, 10, 10, &m_pRWeaponSphere, NULL);
	//피격 범위 표시용 원 설정
	D3DXCreateSphere(g_pD3DDevice, m_stHitSphere.fRadius, 10, 10, &m_pHitSphere, NULL);

	//펠오크 엑스 세팅
	m_pSkinnedMesh = new cSkinnedMesh;
	m_pSkinnedMesh->Setup("Model/Enemy/felorc_axe", "felorc_axe.x");


	int start = 0;
	for (int i = 0; i < g_pGameManager->GetStageOneTile()->GetTileInfo().size(); i++)
	{
		if (g_pGameManager->GetStageOneTile()->GetTileInfo()[i].type == ST_TILE_INFO::MONSTER_SPAWN)
		{
			start = i;
			g_pGameManager->GetStageOneTile()->GetTileInfo()[i].type = ST_TILE_INFO::MONSTER_SPAWN_BLOCK;
			break;
		}
	}

	m_vPosition = g_pGameManager->GetStageOneTile()->GetTileInfo()[start].vecCenter;

	int end = 0;
	for (int i = 0; i < g_pGameManager->GetStageOneTile()->GetTileInfo().size(); i++)
	{
		if (g_pGameManager->GetStageOneTile()->GetTileInfo()[i].type == ST_TILE_INFO::DEST)
		{
			end = i;
			break;
		}
	}
	m_stEndTile = g_pGameManager->GetStageOneTile()->GetTileInfoValue()[end];

	m_vFinalDest = g_pGameManager->GetStageOneTile()->GetTileInfoValue()[end].vecCenter;

	StartThread();
}

void cFelorcAxe::Update()
{
	D3DXMATRIXA16 matS, matR, matT;

	if (!m_isDie)
	{
		if (m_isFindPath)
		{
			cMonster::Update();

			if (m_pSkinnedMesh)
				m_pSkinnedMesh->Update();

			m_vPosition = m_vPosition + (m_vDirection * 0.1f);

			D3DXVECTOR3 v = m_vecDest.back() - m_vPosition;
			float len = D3DXVec3Length(&v);

			if (len <= 0.1f)
			{

				m_vPosition = m_vecDest.back();

				if (m_vPosition == m_vFinalDest)
				{
					m_isDie = true;
					return;
				}

				m_vecDest.pop_back();

				if (m_vecDest.size() == 0)
				{
					m_isFindPath = false;
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

		D3DXMatrixScaling(&matS, 2.0f, 2.0f, 2.0f);
		D3DXMatrixRotationY(&matR, m_fRotY);

		D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
		m_matWorld = matS * matR * matT;
	}
}

void cFelorcAxe::Render()
{
	if (!m_isDie)
	{
		if (m_pSkinnedMesh)
			m_pSkinnedMesh->Render(NULL, &m_matWorld);

		Sphere_Render();
		RWeaponSphere_Render("felorc_axe_Bone103"); //오른쪽 무기 범위 표시
	}
}

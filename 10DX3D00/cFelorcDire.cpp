#include "stdafx.h"
#include "cFelorcDire.h"
#include "cSkinnedMesh.h"

#include "cTile.h"
#include "cCharacter.h"

cFelorcDire::cFelorcDire()
{
}


cFelorcDire::~cFelorcDire()
{
	SAFE_DELETE(m_pSkinnedMesh);
}

void cFelorcDire::Setup()
{
	cMonster::Setup();

	m_vScale = D3DXVECTOR3(3.0f, 3.0f, 3.0f);
	m_vPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//공격 범위 설정
	m_stAttackSphere.fRadius = 5.0f;
	//탐지 범위 설정
	m_stTraceSphere.fRadius = 10.0f;
	//오른쪽 무기 범위 설정
	m_stRWeaponSphere.fRadius = 0.75f;
	//피격 범위 설정
	m_stHitSphere.fRadius = 2.5f;

	//공격 범위 표시용 원 설정
	D3DXCreateSphere(g_pD3DDevice, m_stAttackSphere.fRadius, 10, 10, &m_pAttackSphere, NULL);
	//탐지 범위 표시용 원 설정
	D3DXCreateSphere(g_pD3DDevice, m_stTraceSphere.fRadius, 10, 10, &m_pTraceSphere, NULL);
	//오른쪽 무기 범위 표시용 원 설정
	D3DXCreateSphere(g_pD3DDevice, m_stRWeaponSphere.fRadius, 10, 10, &m_pRWeaponSphere, NULL);
	//피격 범위 표시용 원 설정
	D3DXCreateSphere(g_pD3DDevice, m_stHitSphere.fRadius, 10, 10, &m_pHitSphere, NULL);

	//펠오크 다이어 세팅
	m_pSkinnedMesh = new cSkinnedMesh("Model/Enemy/felorcdire/", "felorcdire.x");

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
	m_stFinalDestTile = g_pGameManager->GetStageOneTile()->GetTileInfoValue()[end];

	StartThread();

	m_nAnimIndex = 4;
	m_pSkinnedMesh->SetAnimationIndexBlend(m_nAnimIndex);
}

void cFelorcDire::Update()
{
	if (m_isDie) return;

	srand(time(NULL));

	if (!AttackSearch())
	{
		if (!m_isTrace)
			TraceSeach();
		else if (m_isTrace)
			CheckTraceTime();
	}

	D3DXMATRIXA16 matS, matR, matT;

	switch (m_eMonsterState)
	{
	case E_IDLE:
	{
		m_nAnimIndex = 4;
		m_pSkinnedMesh->SetNextAniMation(m_nAnimIndex, 4);
		break;
	}
	case E_ATTACK:
	{
		if (m_pSkinnedMesh->AnimationClose(m_nAnimIndex))
		{
			m_eMonsterState = E_NONE;
			m_isAttack = false;
			m_isTrace = false;
			m_isTraceForFinalDest = true;
		}
		break;
	}
	case E_NONE:
	{
		if (m_isFindPath)
		{
			if (m_pSkinnedMesh->SetNextAniMation(0, 0))
				m_eMonsterState = E_NONE;
			Move();
		}
		break;
	}
	case E_DEAD:
	{
		m_nAnimIndex = 3;
		if (m_pSkinnedMesh->SetNextAniMation(m_nAnimIndex, 3))
			m_isDie = true;
		break;
	}
	case E_STUN: //스턴 동작 없음
	{
		m_eMonsterState = E_IDLE;
		break;
	}
	default:
	{
		m_eMonsterState = E_IDLE;
		break;
	}
	}

	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationY(&matR, m_fRotY);

	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matS * matR * matT;

}

void cFelorcDire::Render()
{
	if (!m_isDie)
	{
		cMonster::Render();

		if (m_pSkinnedMesh)
			m_pSkinnedMesh->UpdateAndRender(m_matWorld);

		Sphere_Render(); //탐지, 공격, 피격 범위 표시
		RWeaponSphere_Render("felorcdire_Bone23"); //오른쪽 무기 범위 표시
	}
}

bool cFelorcDire::AttackSearch()
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

				m_nAnimIndex = rand() % 2 + 1;
				m_pSkinnedMesh->SetAnimationIndexBlend(m_nAnimIndex);

				return true;
			}
		}
	}
	else
	{
		for (int i = 0; i < g_pGameManager->GetCharacter().size(); i++)
		{
			//몬스터 무기 범위와 캐릭터 피격 범위 사이의 거리를 구함
			float dist = Distance_Between_Three_Points(g_pGameManager->GetCharacter()[i]->GetHitCollider().vCenter,
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

	return false;
}


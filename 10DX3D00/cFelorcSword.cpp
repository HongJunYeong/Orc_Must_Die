#include "stdafx.h"
#include "cFelorcSword.h"
#include "cSkinnedMesh.h"

#include "cTile.h"
#include "cCharacter.h"

cFelorcSword::cFelorcSword()
{
}


cFelorcSword::~cFelorcSword()
{
	SAFE_DELETE(m_pSkinnedMesh);
}

void cFelorcSword::Setup()
{
	cMonster::Setup();

	//���ũ ������ ����
	m_pSkinnedMesh = new cSkinnedMesh("Model/Enemy/felorc_sword/", "felorc_sword.x");

	m_vScale = D3DXVECTOR3(4.0f, 4.0f, 4.0f);
	m_vPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���� ���� ����
	m_stAttackSphere.fRadius = 4.0f;
	//Ž�� ���� ����
	m_stTraceSphere.fRadius = 10.f;
	//������ ���� ���� ����
	m_stRWeaponSphere.fRadius = 0.5f;
	//�ǰ� ���� ����
	m_stHitSphere.fRadius = 2.4f;

	//���� ���� ǥ�ÿ� �� ����
	D3DXCreateSphere(g_pD3DDevice, m_stAttackSphere.fRadius, 10, 10, &m_pAttackSphere, NULL);
	//Ž�� ���� ǥ�ÿ� �� ����
	D3DXCreateSphere(g_pD3DDevice, m_stTraceSphere.fRadius, 10, 10, &m_pTraceSphere, NULL);
	//������ ���� ���� ǥ�ÿ� �� ����
	D3DXCreateSphere(g_pD3DDevice, m_stRWeaponSphere.fRadius, 10, 10, &m_pRWeaponSphere, NULL);
	//�ǰ� ���� ǥ�ÿ� �� ����
	D3DXCreateSphere(g_pD3DDevice, m_stHitSphere.fRadius, 10, 10, &m_pHitSphere, NULL);

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

	int n = rand() % 2 + 1;
	if (n == 1)
		m_nAnimIndex = 7;
	else if (n == 2)
		m_nAnimIndex = 8;
	m_pSkinnedMesh->SetAnimationIndexBlend(m_nAnimIndex);
}

void cFelorcSword::Update()
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

	//IDLE ���� 2���� �ִϸ��̼� ������ ���� [���� �ִϸ��̼�]
	int n2 = rand() % 2 + 1;
	int nAniTemp;

	if (n2 == 1)
		nAniTemp = 7;
	else if (n2 == 2)
		nAniTemp = 8;

	switch (m_eMonsterState)
	{
	case E_IDLE:
	{
		//E_IDLE ���� 2���� �ִϸ��̼� ������ ����
		int n = rand() % 2 + 1;
		if (n == 1)
			m_nAnimIndex = 7;
		else if (n == 2)
			m_nAnimIndex = 8;

		if (m_pSkinnedMesh->SetNextAniMation(m_nAnimIndex, nAniTemp))
			m_eMonsterState = E_IDLE;
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
			if (m_isGameStart)
			{
				if (m_pSkinnedMesh->SetNextAniMation(0, 0))
					m_eMonsterState = E_NONE;
				Move();
			}
		}
		break;
	}
	case E_DEAD:
	{
		m_nAnimIndex = 6;
		if (m_pSkinnedMesh->SetNextAniMation(m_nAnimIndex, nAniTemp))
			m_isDie = true;
		break;
	}
	case E_STUN:
	{
		m_nAnimIndex = 5;
		if (m_pSkinnedMesh->SetNextAniMation(m_nAnimIndex, nAniTemp))
			m_eMonsterState = E_IDLE;
		break;
	}
	default:
		m_eMonsterState = E_IDLE;
		break;
	}

	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationY(&matR, m_fRotY);

	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matS * matR * matT;
}

void cFelorcSword::Render()
{
	if (!m_isDie)
	{
		cMonster::Render();

		if (m_pSkinnedMesh)
			m_pSkinnedMesh->UpdateAndRender(m_matWorld);
		Sphere_Render(); //Ž��, ����, �ǰ� ���� ǥ��
		RWeaponSphere_Render("felorc_sword_Bone103"); //������ ���� ���� ǥ��
	}
}

bool cFelorcSword::AttackSearch()
{
	if (!m_isAttack)
	{
		for (int i = 0; i < g_pGameManager->GetCharacter().size(); i++)
		{
			float dist = Distance_Between_Three_Points(g_pGameManager->GetCharacter()[i]->GetHitCollider().vCenter,
				m_stHitSphere.vCenter);

			//�� �� ������ �Ÿ� ������ ���� ���� �ǰ� ������ ĳ������ �ǰ� ������ �浹�Ǿ����� �ľ�
			if (dist <= (m_stHitSphere.fRadius
				+ g_pGameManager->GetCharacter()[i]->GetHitCollider().fRadius))
			{
				m_eMonsterState = E_ATTACK;
				m_isAttack = true;

				m_nAnimIndex = rand() % 4 + 1;
				m_pSkinnedMesh->SetAnimationIndexBlend(m_nAnimIndex);

				return true;
			}
		}
	}
	else
	{
		for (int i = 0; i < g_pGameManager->GetCharacter().size(); i++)
		{
			//���� ���� ������ ĳ���� �ǰ� ���� ������ �Ÿ��� ����
			float dist = Distance_Between_Three_Points(g_pGameManager->GetCharacter()[i]->GetHitCollider().vCenter,
				m_stRWeaponSphere.vCenter);

			//���� ���� ������ ĳ���� �ǰ� ������ �浹�Ǿ����� �ľ�
			if (dist <= (m_stRWeaponSphere.fRadius
				+ g_pGameManager->GetCharacter()[i]->GetHitCollider().fRadius))
			{
				//cout << "�浹������" << endl;
				return true;
			}
		}
	}

	return false;
}
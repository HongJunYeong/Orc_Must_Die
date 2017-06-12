#include "stdafx.h"
#include "cFelorcWarriorBoss.h"
#include "cSkinnedMesh.h"

#include "cTile.h"
#include "cCharacter.h"

cFelorcWarriorBoss::cFelorcWarriorBoss()
{
}


cFelorcWarriorBoss::~cFelorcWarriorBoss()
{
	SAFE_DELETE(m_pSkinnedMesh);
}

void cFelorcWarriorBoss::Setup()
{
	cMonster::Setup();

	m_vScale = D3DXVECTOR3(4.0f, 4.0f, 4.0f);
	m_vPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���� ���� ����
	m_stAttackSphere.fRadius = 4.0f;
	//Ž�� ���� ����
	m_stTraceSphere.fRadius = 15.0f;
	//������ ���� ���� ����
	m_stRWeaponSphere.fRadius = 0.75f;
	//���� ���� ���� ����
	m_stLWeaponSphere.fRadius = 0.75f;
	//�ǰ� ���� ����
	m_stHitSphere.fRadius = 2.4f;

	//���� ���� ǥ�ÿ� �� ����
	D3DXCreateSphere(g_pD3DDevice, m_stAttackSphere.fRadius, 10, 10, &m_pAttackSphere, NULL);
	//Ž�� ���� ǥ�ÿ� �� ����
	D3DXCreateSphere(g_pD3DDevice, m_stTraceSphere.fRadius, 10, 10, &m_pTraceSphere, NULL);
	//������ ���� ���� ǥ�ÿ� �� ����
	D3DXCreateSphere(g_pD3DDevice, m_stRWeaponSphere.fRadius, 10, 10, &m_pRWeaponSphere, NULL);
	//���� ���� ���� ǥ�ÿ� �� ����
	D3DXCreateSphere(g_pD3DDevice, m_stLWeaponSphere.fRadius, 10, 10, &m_pLWeaponSphere, NULL);
	//�ǰ� ���� ǥ�ÿ� �� ����
	D3DXCreateSphere(g_pD3DDevice, m_stHitSphere.fRadius, 10, 10, &m_pHitSphere, NULL);

	//���ũ ������ ���� ����
	m_pSkinnedMesh = new cSkinnedMesh("Model/Enemy/felorcwarriorboss/", "felorcwarriorboss.x");

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
	//m_vFinalDest =g_pGameManager->GetStageOneTile()->GetTileInfoValue()[end].vecCenter;
	StartThread();

	int n = rand() % 3 + 1;
	if (n == 1)
		m_nAnimIndex = 3;
	else if (n == 2)
		m_nAnimIndex = 4;
	else if (n == 3)
		m_nAnimIndex = 9;
	m_pSkinnedMesh->SetAnimationIndexBlend(m_nAnimIndex);
}

void cFelorcWarriorBoss::Update()
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

	//IDLE ���� 3���� �ִϸ��̼� ������ ���� [���� �ִϸ��̼�]
	int n2 = rand() % 3 + 1;
	int nAniTemp;

	if (n2 == 1)
		nAniTemp = 3;
	else if (n2 == 2)
		nAniTemp = 4;
	else if (n2 == 3)
		nAniTemp = 9;

	switch (m_eMonsterState)
	{
	case E_IDLE:
	{
		//E_IDLE ���� 3���� �ִϸ��̼� ������ ����
		int n = rand() % 3 + 1;
		if (n == 1)
			m_nAnimIndex = 3;
		else if (n == 2)
			m_nAnimIndex = 4;
		else if (n == 3)
			m_nAnimIndex = 9;

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
			if (m_pSkinnedMesh->SetNextAniMation(5, 5))
				m_eMonsterState = E_NONE;
			Move();
		}
		break;
	}
	case E_DEAD:
	{
		m_nAnimIndex = 8;
		if (m_pSkinnedMesh->SetNextAniMation(m_nAnimIndex, nAniTemp))
			m_isDie = true;
		break;
	}
	case E_STUN: //���� ���� ����
	{
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

void cFelorcWarriorBoss::Render()
{
	if (!m_isDie)
	{
		cMonster::Render();

		if (m_pSkinnedMesh)
			m_pSkinnedMesh->UpdateAndRender(m_matWorld);
		Sphere_Render(); //Ž��, ����, �ǰ� ���� ǥ��
		RWeaponSphere_Render("felorcwarriorboss_Bone110"); //������ ���� ���� ǥ��
		LWeaponSphere_Render("felorcwarriorboss_Bone103");//���� ���� ���� ǥ��
	}
}

bool cFelorcWarriorBoss::AttackSearch()
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

				//E_ATTACK ���� 4���� �ִϸ��̼� ������ ����
				int n = rand() % 4 + 1;
				if (n == 1)
					m_nAnimIndex = 0;
				else if (n == 2)
					m_nAnimIndex = 1;
				else if (n == 3)
					m_nAnimIndex = 6;
				else if (n == 4)
					m_nAnimIndex = 7;
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


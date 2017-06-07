#include "stdafx.h"
#include "cFelorcAxe.h"

#include "cSkinnedMesh.h"

cFelorcAxe::cFelorcAxe()
{
}


cFelorcAxe::~cFelorcAxe()
{
}

void cFelorcAxe::Setup()
{
	cMonster::Setup();

	m_pSkinnedMesh = new cSkinnedMesh;
	m_pSkinnedMesh->Setup("Model/Enemy/felorc_axe", "felorc_axe.x");


}

void cFelorcAxe::Update()
{
	if (m_pSkinnedMesh)
		m_pSkinnedMesh->Update();
}

void cFelorcAxe::Render()
{
	if (m_pSkinnedMesh)
		m_pSkinnedMesh->Render(NULL,D3DXVECTOR3(2.0f,2.0f,2.0f));
}

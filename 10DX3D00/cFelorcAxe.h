#pragma once

#include "cMonster.h"

class cFelorcAxe : public cMonster
{
public:
	cFelorcAxe();
	virtual ~cFelorcAxe();

	ST_SPHERE		m_stTraceSphere;
	ST_SPHERE		m_stAttackSphere;


public:
	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
};


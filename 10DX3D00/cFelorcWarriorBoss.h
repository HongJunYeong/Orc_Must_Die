#pragma once
#include "cMonster.h"
class cFelorcWarriorBoss : public cMonster
{
public:
	cFelorcWarriorBoss();
	virtual ~cFelorcWarriorBoss();

public:
	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual bool AttackSearch() override;

};


#pragma once
#include "cMonster.h"
class cFelorcWarriorSword : public cMonster
{
public:
	cFelorcWarriorSword();
	virtual ~cFelorcWarriorSword();

public:
	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual bool AttackSearch() override;
};


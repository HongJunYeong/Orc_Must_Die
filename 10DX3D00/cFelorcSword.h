#pragma once
#include "cMonster.h"
class cFelorcSword : public cMonster
{
public:
	cFelorcSword();
	virtual ~cFelorcSword();

public:
	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual bool AttackSearch() override;

};


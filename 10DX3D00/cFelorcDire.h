#pragma once
#include "cMonster.h"

class cFelorcDire : public cMonster
{
public:
	cFelorcDire();
	virtual ~cFelorcDire();
public:
	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual bool AttackSearch() override;

};


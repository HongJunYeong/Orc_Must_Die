#pragma once

#include "cMonster.h"

class cFelorcAxe : public cMonster
{
public:
	cFelorcAxe();
	virtual ~cFelorcAxe();
public:
	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual bool AttackSearch() override;
};


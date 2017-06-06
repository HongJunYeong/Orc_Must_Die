#pragma once

class cSkinnedMesh;

class cMonster
{
public:
	cMonster();
	virtual ~cMonster();

protected:
	cSkinnedMesh*		m_pSkinnedMesh;

	bool				m_isDie;
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};


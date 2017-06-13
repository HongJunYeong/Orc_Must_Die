#pragma once
class cCharacter
{
public:
	cCharacter();
	virtual ~cCharacter();

protected:
	ST_SPHERE		m_hitColider;
	D3DXVECTOR3		m_vDirection;
	LPD3DXMESH		m_pHitColliderSpehre;
	D3DXMATRIXA16	m_matWorld;

	float			m_fRotY;
public:

	D3DXVECTOR3		m_vPostion;

	virtual void Setup();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	virtual ST_SPHERE& GetHitCollider() { return m_hitColider; }
};


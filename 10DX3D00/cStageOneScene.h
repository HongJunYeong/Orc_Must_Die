#pragma once
#include "cScene.h"

class cMtlTex;
class xFileToMesh;

class cStageOneScene : public cScene
{
public:
	cStageOneScene();
	virtual ~cStageOneScene();

private:
	vector<ST_OBJECT*>				m_vecObj;
	ST_OBJECT_INFO*					m_pStObject; // ��������� ����� ����ü ������
	map<string, ST_OBJECT_INFO*>    m_mapObj; // ��Ʈ������ ������ ����
	LPD3DXMESH						m_pObjMesh;
	vector<cMtlTex*>				m_vecObjMtlTex;

	D3DMATERIAL9					m_stMtl;

public:
	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
	virtual void Release() override;
	virtual void StartRecvThread() override;

	//
	void ObjSetup();
	void ObjRender();
};


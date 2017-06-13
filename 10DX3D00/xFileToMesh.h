#pragma once
class xFileToMesh
{
public:
	xFileToMesh();
	~xFileToMesh();

private:
	LPD3DXMESH              m_pMesh = NULL; // �޽� ��ü
	D3DMATERIAL9*           m_pMeshMaterials = NULL; // �޽ÿ��� ����� ����
	LPDIRECT3DTEXTURE9*     m_pMeshTextures = NULL; // �޽ÿ��� ����� �ؽ���
	DWORD                   m_dwNumMaterials = 0L;   // �޽ÿ��� ������� ������ ����

public:
	ST_OBJECT_INFO* Load(string szFolder, string szFileName);
};


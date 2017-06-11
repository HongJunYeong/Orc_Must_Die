#pragma once
class xFileToMesh
{
public:
	xFileToMesh();
	~xFileToMesh();

private:
	LPD3DXMESH              m_pMesh = NULL; // 메시 객체
	D3DMATERIAL9*           m_pMeshMaterials = NULL; // 메시에서 사용할 재질
	LPDIRECT3DTEXTURE9*     m_pMeshTextures = NULL; // 메시에서 사용할 텍스쳐
	DWORD                   m_dwNumMaterials = 0L;   // 메시에서 사용중인 재질의 개수

public:
	ST_OBJECT_INFO* Load(string szFolder, string szFileName);
};


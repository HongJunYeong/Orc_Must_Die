#include "stdafx.h"
#include "xFileToMesh.h"


xFileToMesh::xFileToMesh()
{
}


xFileToMesh::~xFileToMesh()
{
}


ST_OBJECT_INFO* xFileToMesh::Load(string szFolder, string szFileName)
{
	string szFullPath = szFolder + szFileName;
	/// 재질을 임시로 보관할 버퍼선언
	LPD3DXBUFFER pD3DXMtrlBuffer;

	/// x파일을 메시로 읽어들인다. 이때 재질정보도 함께 읽는다.
	if (FAILED(D3DXLoadMeshFromX(szFullPath.c_str(), D3DXMESH_SYSTEMMEM,
		g_pD3DDevice, NULL,
		&pD3DXMtrlBuffer, NULL, &m_dwNumMaterials,
		&m_pMesh)))
	{
		MessageBox(NULL, "Could not find XFile", "제목.exe", MB_OK);
	}

	/// 재질정보와 텍스쳐 정보를 따로 뽑아낸다. 
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];			/// 재질개수만큼 재질구조체 배열 생성
	m_pMeshTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];	/// 재질개수만큼 텍스쳐 배열 생성

	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		/// 재질정보를 복사
		m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		/// 주변광원정보를 Diffuse정보로
		m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;
		m_pMeshTextures[i] = NULL;

		if (d3dxMaterials[i].pTextureFilename != NULL && lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{	
			/// 텍스쳐를 파일에서 로드한다
			if (FAILED(D3DXCreateTextureFromFile(g_pD3DDevice,
				d3dxMaterials[i].pTextureFilename,
				&m_pMeshTextures[i])))
			{
				/// 텍스쳐가 현재 폴더에 없으면 상위폴더 검색
				const TCHAR* strPrefix = TEXT(szFolder.c_str());
				const int lenPrefix = lstrlen(strPrefix);

				/// MAX_PATH 는 260. API상에서 260자를 넘는 인자가 들어오면 올바르지않는 인수로 
				TCHAR strTexture[MAX_PATH];
				lstrcpyn(strTexture, strPrefix, MAX_PATH);
				lstrcpyn(strTexture + lenPrefix, d3dxMaterials[i].pTextureFilename, MAX_PATH - lenPrefix);
				if (FAILED(D3DXCreateTextureFromFile(g_pD3DDevice,
					strTexture,
					&m_pMeshTextures[i])))
				{
					MessageBox(NULL, "Could not find texture map", "제목.exe", MB_OK);
				}
			}
		}
	}

	/// 임시로 생성한 재질버퍼 소거
	pD3DXMtrlBuffer->Release();

	ST_OBJECT_INFO* obj;
	obj = new ST_OBJECT_INFO;

	obj->_pMesh = m_pMesh;
	obj->_pMeshMaterials = m_pMeshMaterials;
	obj->_pMeshTextures = m_pMeshTextures;
	obj->_dwNumMaterials = m_dwNumMaterials;
	return obj;
}
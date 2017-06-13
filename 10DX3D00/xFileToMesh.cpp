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
	/// ������ �ӽ÷� ������ ���ۼ���
	LPD3DXBUFFER pD3DXMtrlBuffer;

	/// x������ �޽÷� �о���δ�. �̶� ���������� �Բ� �д´�.
	if (FAILED(D3DXLoadMeshFromX(szFullPath.c_str(), D3DXMESH_SYSTEMMEM,
		g_pD3DDevice, NULL,
		&pD3DXMtrlBuffer, NULL, &m_dwNumMaterials,
		&m_pMesh)))
	{
		MessageBox(NULL, "Could not find XFile", "����.exe", MB_OK);
	}

	/// ���������� �ؽ��� ������ ���� �̾Ƴ���. 
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];			/// ����������ŭ ��������ü �迭 ����
	m_pMeshTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];	/// ����������ŭ �ؽ��� �迭 ����

	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		/// ���������� ����
		m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		/// �ֺ����������� Diffuse������
		m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;
		m_pMeshTextures[i] = NULL;

		if (d3dxMaterials[i].pTextureFilename != NULL && lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{	
			/// �ؽ��ĸ� ���Ͽ��� �ε��Ѵ�
			if (FAILED(D3DXCreateTextureFromFile(g_pD3DDevice,
				d3dxMaterials[i].pTextureFilename,
				&m_pMeshTextures[i])))
			{
				/// �ؽ��İ� ���� ������ ������ �������� �˻�
				const TCHAR* strPrefix = TEXT(szFolder.c_str());
				const int lenPrefix = lstrlen(strPrefix);

				/// MAX_PATH �� 260. API�󿡼� 260�ڸ� �Ѵ� ���ڰ� ������ �ùٸ����ʴ� �μ��� 
				TCHAR strTexture[MAX_PATH];
				lstrcpyn(strTexture, strPrefix, MAX_PATH);
				lstrcpyn(strTexture + lenPrefix, d3dxMaterials[i].pTextureFilename, MAX_PATH - lenPrefix);
				if (FAILED(D3DXCreateTextureFromFile(g_pD3DDevice,
					strTexture,
					&m_pMeshTextures[i])))
				{
					MessageBox(NULL, "Could not find texture map", "����.exe", MB_OK);
				}
			}
		}
	}

	/// �ӽ÷� ������ �������� �Ұ�
	pD3DXMtrlBuffer->Release();

	ST_OBJECT_INFO* obj;
	obj = new ST_OBJECT_INFO;

	obj->_pMesh = m_pMesh;
	obj->_pMeshMaterials = m_pMeshMaterials;
	obj->_pMeshTextures = m_pMeshTextures;
	obj->_dwNumMaterials = m_dwNumMaterials;
	return obj;
}
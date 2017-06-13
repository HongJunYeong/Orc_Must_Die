#include "StdAfx.h"
#include "cSkinnedMeshManager.h"
#include "cSkinnedMesh.h"

cSkinnedMeshManager::cSkinnedMeshManager(void)
{
}


cSkinnedMeshManager::~cSkinnedMeshManager(void)
{
}

void cSkinnedMeshManager::Destroy()
{
	for each(auto it in m_mapSkinnedMesh)
	{
		it.second->Destroy();
		SAFE_DELETE(it.second);
	}
}

cSkinnedMesh* cSkinnedMeshManager::GetSkinnedMesh( char* szFolder, char* szFilename )
{
	std::string sFullPath(szFolder);
	sFullPath += std::string(szFilename);

	if(m_mapSkinnedMesh.find(sFullPath) == m_mapSkinnedMesh.end())
	{
		cSkinnedMesh* pSkinnedMesh = new cSkinnedMesh();
		pSkinnedMesh->Load(szFolder, szFilename);
		m_mapSkinnedMesh[sFullPath] = pSkinnedMesh;
	}
	return m_mapSkinnedMesh[sFullPath];
}

void cSkinnedMeshManager::AddSkinnedMesh(char * szFolder, char * szFileName)
{
	std::string sFullPath(szFolder);
	sFullPath += std::string(szFileName);

	cSkinnedMesh* pSkinnedMesh = new cSkinnedMesh();
	pSkinnedMesh->Load(szFolder, szFileName);

	m_mapSkinnedMesh.insert(pair<string, cSkinnedMesh*>(sFullPath, pSkinnedMesh));
}

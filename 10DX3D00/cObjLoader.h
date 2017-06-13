#pragma once

class cGroup;
class cMtlTex;

class cObjLoader
{
public:
	cObjLoader();
	~cObjLoader();

private:
	map<string, cMtlTex*> m_mapMtlTex;

public:
	void Load(OUT vector<cGroup*> & vectorGroup, IN char* szFolder, IN char* szFile);
	void LoadSurface(OUT vector<D3DXVECTOR3>& vecSurface, IN char* szFolder, IN char* szFile, D3DXMATRIXA16* pMat = NULL);
	void LoadMtlLib(char* szFolder, char* szFile);

	LPD3DXMESH LoadMesh(OUT vector<cMtlTex*> & vecMtlTex, IN char* szFolder, IN char* szFile);
};


#include "stdafx.h"
#include "cStageOneScene.h"

#include "cTile.h"
#include "cObjLoader.h"
#include "xFileToMesh.h"
#include "cMtlTex.h"


cStageOneScene::cStageOneScene()
{
}


cStageOneScene::~cStageOneScene()
{
}

void cStageOneScene::Setup()
{
	ObjSetup();
	{
		vector<ST_OBJECT*>::iterator iter;
		for (iter = m_vecObj.begin(); iter != m_vecObj.end(); ++iter)
		{
			delete(*iter);
		}
		m_vecObj.clear();

		{
			FILE *fp;
			fopen_s(&fp, "ObjMap.txt", "r");

			ST_OBJECT stTemp;

			while (true)
			{
				if (feof(fp)) break;

				char szTemp[1024];
				fgets(szTemp, 1024, fp);

				if (szTemp[0] == 'n')
				{
					char st[100];
					string strr = "";
					sscanf_s(szTemp, "%*s %s", &st, 100);

					stTemp.sObjName = st;
				}

				if (szTemp[0] == 't')
				{
					int n;
					sscanf_s(szTemp, "%*s %d", &n);
					stTemp.eType = (ST_OBJECT::eTYPE)n;
				}
				if (stTemp.eType != ST_OBJECT::eTYPE::OBJ) continue;

				if (szTemp[0] == 'r')
				{
					float f;
					sscanf_s(szTemp, "%*s %f", &f);
					stTemp.fRot = f;
				}

				if (szTemp[0] == 'i')
				{
					int n;
					sscanf_s(szTemp, "%*s %d", &n);
					stTemp.idX = n;
				}
				if (szTemp[0] == 'j')
				{
					int n;
					sscanf_s(szTemp, "%*s %d", &n);
					stTemp.idY = n;
				}
				if (szTemp[0] == 'x')
				{
					float f;
					sscanf_s(szTemp, "%*s %f", &f);
					stTemp.vCenter.x = f;
				}
				if (szTemp[0] == 'y')
				{
					float f;
					sscanf_s(szTemp, "%*s %f", &f);
					stTemp.vCenter.y = f;
				}
				if (szTemp[0] == 'z')
				{
					float f;
					sscanf_s(szTemp, "%*s %f", &f);
					stTemp.vCenter.z = f;
				}
				if (szTemp[0] == 'c')
				{
					ST_OBJECT* b = new ST_OBJECT;
					b->sObjName = stTemp.sObjName;
					b->fRot = stTemp.fRot;
					b->eType = stTemp.eType;
					b->idX = stTemp.idX;
					b->idY = stTemp.idY;

					b->vCenter.x = stTemp.vCenter.x;
					b->vCenter.y = stTemp.vCenter.y;
					b->vCenter.z = stTemp.vCenter.z;
					m_vecObj.push_back(b);
				}
			}
			fclose(fp);
		}
	}
}

void cStageOneScene::Update()
{
	g_pGameManager->StageOneUpdate();
}

void cStageOneScene::Render(LPD3DXSPRITE pSprite)
{
	{
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
		ObjRender();

		D3DXMATRIXA16 matT, matS, matR, matWorld;

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matT);

		D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);
		D3DXMatrixTranslation(&matT, 248.0f, 51.2f, -120.0f);

		matWorld = matS * matT;
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

		for (size_t i = 0; i < m_vecObjMtlTex.size(); i++)
		{
			g_pD3DDevice->SetMaterial(&m_vecObjMtlTex[i]->GetMaterial());
			g_pD3DDevice->SetTexture(0, m_vecObjMtlTex[i]->GetTexture());
			m_pObjMesh->DrawSubset(i);
		}
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	}
	g_pGameManager->StageOneRender();
}

void cStageOneScene::Release()
{
	SAFE_RELEASE(m_pObjMesh);
}

void cStageOneScene::StartRecvThread()
{
}

void cStageOneScene::ObjSetup()
{
	cObjLoader load;

	m_pObjMesh = load.LoadMesh(m_vecObjMtlTex, "Model/Obj/Sepia_Hallway.unr", "Sepia_Hallway.obj");
	//
	{
		xFileToMesh l;

		// 오브젝트 초기화
		m_pStObject = l.Load(string("Model/Object/altartidalmastery01/"), string("altartidalmastery01.X"));
		m_pStObject->_fScale = 1.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("altartidalmastery01", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/ao_barrel01/"), string("ao_barrel01.X"));
		m_pStObject->_fScale = 3.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("ao_barrel01", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/ao_boardwalk01/"), string("ao_boardwalk01.X"));
		m_pStObject->_fScale = 0.5f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("ao_boardwalk01", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/bt_brazier/"), string("bt_brazier.X"));
		m_pStObject->_fScale = 5.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("bt_brazier", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/bt_brazier_broken/"), string("bt_brazier_broken.X"));
		m_pStObject->_fScale = 5.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("bt_brazier_broken", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/centaur_brokepillar01/"), string("centaur_brokepillar01.X"));
		m_pStObject->_fScale = 2.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("centaur_brokepillar01", m_pStObject));

		//-------------------------------------------

		m_pStObject = l.Load(string("Model/Object/centaur_pillar01/"), string("centaur_pillar01.X"));
		m_pStObject->_fScale = 2.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("centaur_pillar01", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/centaur_wall_ruin01/"), string("centaur_wall_ruin01.X"));
		m_pStObject->_fScale = 3.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("centaur_wall_ruin01", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/centaur_wall01/"), string("centaur_wall01.X"));
		m_pStObject->_fScale = 3.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("centaur_wall01", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/cf_elevatorplatform/"), string("cf_elevatorplatform.X"));
		m_pStObject->_fScale = 0.5; // 포지션.y = -0.36f
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("cf_elevatorplatform", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/cf_groundcage/"), string("cf_groundcage.X"));
		m_pStObject->_fScale = 1.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("cf_groundcage", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/ghostland_obelisk_01/"), string("ghostland_obelisk_01.X"));
		m_pStObject->_fScale = 2.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("ghostland_obelisk_01", m_pStObject));

		//------------------------------------------


		m_pStObject = l.Load(string("Model/Object/ghostland_obelisk_02/"), string("ghostland_obelisk_02.X"));
		m_pStObject->_fScale = 2.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("ghostland_obelisk_02", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/helpwantedposter/"), string("helpwantedposter.X"));
		m_pStObject->_fScale = 6.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("helpwantedposter", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/lo_well_01/"), string("lo_well_01.X"));
		m_pStObject->_fScale = 0.5f;  //////////////////
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("lo_well_01", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/murlocaltar/"), string("murlocaltar.X"));
		m_pStObject->_fScale = 6.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("murlocaltar", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/ogrila_banner/"), string("ogrila_banner.X"));
		m_pStObject->_fScale = 4.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("ogrila_banner", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/zangarmushroom06_blueglow/"), string("zangarmushroom06_blueglow.X"));
		m_pStObject->_fScale = 1.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("zangarmushroom06_blueglow", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/zulgurubpendoor/"), string("zulgurubpendoor.X"));
		m_pStObject->_fScale = 2.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("zulgurubpendoor", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/centaur_arch01/"), string("centaur_arch01.X"));
		m_pStObject->_fScale = 2.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("centaur_arch01", m_pStObject));

		m_pStObject = l.Load(string("Model/Object/centaur_brokearch02/"), string("centaur_brokearch02.X"));
		m_pStObject->_fScale = 2.0f;
		m_mapObj.insert(pair<string, ST_OBJECT_INFO*>("centaur_brokearch02", m_pStObject));
	}
}

void cStageOneScene::ObjRender()
{

	D3DXMATRIXA16 matWorld, matS, matR, matT;

	for each(auto obj in m_vecObj)
	{
		ST_OBJECT_INFO objInfo;

		objInfo = *m_mapObj[obj->sObjName];

		D3DXMatrixScaling(&matS, objInfo._fScale, objInfo._fScale, objInfo._fScale);
		D3DXMatrixRotationY(&matR, obj->fRot);
		D3DXMatrixTranslation(&matT, obj->vCenter.x, obj->vCenter.y, obj->vCenter.z);

		matWorld = matS * matR * matT;

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

		for (size_t i = 0; i < objInfo._dwNumMaterials; i++)
		{
			if (objInfo._pMeshTextures)
			{
				g_pD3DDevice->SetTexture(0, objInfo._pMeshTextures[i]);
			}
		    else g_pD3DDevice->SetMaterial(&objInfo._pMeshMaterials[i]);

			objInfo._pMesh->DrawSubset(i);
		}
	}

}

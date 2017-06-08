#include "stdafx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"

cSkinnedMesh::cSkinnedMesh()
	: m_pRoot(NULL)
	, m_pAnimController(NULL)
	,m_fBlendTime(0.3f)
	,m_fPassedBlendTime(0.0f)
	,m_isAnimBlend(false)
{
}


cSkinnedMesh::~cSkinnedMesh()
{
	cAllocateHierarchy ah;
	D3DXFrameDestroy(m_pRoot, &ah);
	SAFE_RELEASE(m_pAnimController);
}

void cSkinnedMesh::Setup(char* szFolder, char* szFile)
{
	std::string sFullPath(szFolder);
	sFullPath = sFullPath + std::string("/") + std::string(szFile);

	cAllocateHierarchy ah;
	ah.SetFolder(szFolder);

	D3DXLoadMeshHierarchyFromX(sFullPath.c_str(),
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&ah,
		NULL,
		&m_pRoot,
		&m_pAnimController);

	SetupBoneMatrixPtrs(m_pRoot);
}

void cSkinnedMesh::Update()
{
	//	>>:
	if (m_isAnimBlend)
	{
		m_fPassedBlendTime += g_pTimeManager->GetElapsedTime();
		if (m_fPassedBlendTime >= m_fBlendTime)
		{
			m_isAnimBlend = false;
			m_pAnimController->SetTrackWeight(0, 1.0f);
			m_pAnimController->SetTrackEnable(1, false);
		}
		else
		{
			float fWeight = m_fPassedBlendTime / m_fBlendTime;
			m_pAnimController->SetTrackWeight(0, fWeight);
			m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
		}
	}
	//	<<:
	m_pAnimController->AdvanceTime(g_pTimeManager->GetElapsedTime(), NULL);
	Update(m_pRoot, NULL);
	UpdateSkinnedMesh(m_pRoot);
}

void cSkinnedMesh::Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
	if (pFrame == NULL)
		pFrame = m_pRoot;

	ST_BONE* pBone = (ST_BONE*)pFrame;
	pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;
	if (pParent)
	{
		pBone->CombinedTransformationMatrix *=
			((ST_BONE*)pParent)->CombinedTransformationMatrix;
	}

	if (pFrame->pFrameFirstChild)
	{
		Update(pFrame->pFrameFirstChild, pFrame);
	}

	if (pFrame->pFrameSibling)
	{
		Update(pFrame->pFrameSibling, pParent);
	}
}

void cSkinnedMesh::Render(LPD3DXFRAME pFrame, D3DXMATRIXA16* matWorld)
{
	if (pFrame == NULL)
		pFrame = m_pRoot;

	ST_BONE* pBone = (ST_BONE*)pFrame;

	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		if (pBoneMesh->MeshData.pMesh)
		{
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &(pBone->CombinedTransformationMatrix * (*matWorld)));

			for (size_t i = 0; i < pBoneMesh->vecMtl.size(); ++i)
			{
				g_pD3DDevice->SetTexture(0, pBoneMesh->vecTexture[i]);
				g_pD3DDevice->SetMaterial(&pBoneMesh->vecMtl[i]);
				pBoneMesh->MeshData.pMesh->DrawSubset(i);
			}
		}
	}

	if (pFrame->pFrameFirstChild)
	{
		Render(pFrame->pFrameFirstChild, matWorld);
	}

	if (pFrame->pFrameSibling)
	{
		Render(pFrame->pFrameSibling, matWorld);
	}
}

void cSkinnedMesh::SetupBoneMatrixPtrs(LPD3DXFRAME pFrame)
{
	if (pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh =
			(ST_BONE_MESH*)pFrame->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				ST_BONE* pBone =
					(ST_BONE*)D3DXFrameFind(m_pRoot, pSkinInfo->GetBoneName(i));
				pBoneMesh->ppBoneMatrixPtrs[i] =
					&(pBone->CombinedTransformationMatrix);
			}
		}
	}

	if (pFrame->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs(pFrame->pFrameFirstChild);
	}

	if (pFrame->pFrameSibling)
	{
		SetupBoneMatrixPtrs(pFrame->pFrameSibling);
	}
}

void cSkinnedMesh::UpdateSkinnedMesh(LPD3DXFRAME pFrame)
{
	if (pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh =
			(ST_BONE_MESH*)pFrame->pMeshContainer;

		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				pBoneMesh->pCurrentBoneMatrices[i] =
					pBoneMesh->pBoneOffsetMatrices[i] * *(pBoneMesh->ppBoneMatrixPtrs[i]);
			}
		}

		BYTE* src = NULL;
		BYTE* dest = NULL;

		pBoneMesh->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&src);
		pBoneMesh->MeshData.pMesh->LockVertexBuffer(0, (void**)&dest);

		pBoneMesh->pSkinInfo->UpdateSkinnedMesh(
			pBoneMesh->pCurrentBoneMatrices, NULL, src, dest);

		pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();
		pBoneMesh->pOrigMesh->UnlockVertexBuffer();
	}

	if (pFrame->pFrameFirstChild)
	{
		UpdateSkinnedMesh(pFrame->pFrameFirstChild);
	}

	if (pFrame->pFrameSibling)
	{
		UpdateSkinnedMesh(pFrame->pFrameSibling);
	}
}

void cSkinnedMesh::SetAnimationIndex(int nIndex)
{
	int num = m_pAnimController->GetNumAnimationSets();

	if (nIndex > num) nIndex = nIndex % num;

	LPD3DXANIMATIONSET pAnimSet = NULL;
	m_pAnimController->GetAnimationSet(nIndex, &pAnimSet);
	m_pAnimController->SetTrackAnimationSet(0, pAnimSet);
	
	m_pAnimController->GetPriorityBlend();

	SAFE_RELEASE(pAnimSet);
}

void cSkinnedMesh::SetAnimationIndexBlend(int nIndex)
{
	m_isAnimBlend = true;
	m_fPassedBlendTime = 0.0f;

	int num = m_pAnimController->GetNumAnimationSets();
	if (nIndex > num) nIndex = nIndex % num;

	LPD3DXANIMATIONSET pPrevAnimSet = NULL;
	LPD3DXANIMATIONSET pNexetAnimSet = NULL;

	D3DXTRACK_DESC stTrackDesc;
	m_pAnimController->GetTrackDesc(0, &stTrackDesc);

	m_pAnimController->GetTrackAnimationSet(0, &pPrevAnimSet);
	m_pAnimController->SetTrackAnimationSet(1, pPrevAnimSet);
	m_pAnimController->SetTrackDesc(1, &stTrackDesc);

	m_pAnimController->GetAnimationSet(nIndex, &pNexetAnimSet);
	m_pAnimController->SetTrackAnimationSet(0, pNexetAnimSet);
	m_pAnimController->SetTrackPosition(0, 0.0f);

	m_pAnimController->SetTrackWeight(0, 0.0f);
	m_pAnimController->SetTrackWeight(1, 1.0f);
	//m_pAnimController->ResetTime();

	SAFE_RELEASE(pPrevAnimSet);
	SAFE_RELEASE(pNexetAnimSet);
}

LPD3DXFRAME cSkinnedMesh::GetRootFrame()
{
	return m_pRoot;
}

void cSkinnedMesh::SetRootFrame(LPD3DXFRAME pFrame)
{
	m_pRoot = pFrame;
}

double cSkinnedMesh::GetAniTime(int nindex)
{
	//애니메이션의 총 갯수를 넘은 index값이 들어왔을 경우 재설정
	int num = m_pAnimController->GetNumAnimationSets();
	if (nindex > num) nindex = nindex % num;

	//애니메이션 구간 얻기
	LPD3DXANIMATIONSET pAniSet = nullptr;
	m_pAnimController->GetAnimationSet(nindex, &pAniSet); //해당 인덱스의 애니메이션을 저장
	double dPeriod = pAniSet->GetPeriod();//index위치의 애니메이션 길이를 얻은 후 pPeriod에 저장

	SAFE_RELEASE(pAniSet);

	return dPeriod;
}

double cSkinnedMesh::GetAniTrackPosition()
{
	//애니메이션 진행도 얻기
	D3DXTRACK_DESC pTrackDesc;
	m_pAnimController->GetTrackDesc(0, &pTrackDesc); //현재 진행중인 트랙을 가져옴
	double dPosition = pTrackDesc.Position; //현재 진행중인 트랙의 위치(진행도)를 얻음 [로컬 프레임 타임]

	return dPosition;
}

void cSkinnedMesh::SetNextAniMation(int nIndex, int nNextIndex)
{
	int num = m_pAnimController->GetNumAnimationSets();
	if (nIndex > num) nIndex = nIndex % num;

	double dPeriod = GetAniTime(nIndex); //현재 애니메이션의 총 길이를 구함
	double dPeridPosition = GetAniTrackPosition(); //현재 애니메이션의 위치를 구함

	if ((dPeridPosition + 0.001f) >= dPeriod)
	{
		SetAnimationIndexBlend(nNextIndex);
	}
}
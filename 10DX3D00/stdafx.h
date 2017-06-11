// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here

#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
#include <assert.h>
#include <functional>
#include <process.h>
#include <WinSock2.h>
#include <iostream>
#include <time.h>
#include <thread>

#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "ws2_32.lib")

using namespace std;

extern HWND	g_hWnd;
extern HINSTANCE g_hInstance;
#define SAFE_RELEASE(p) { if(p) p->Release(); p = NULL; }
#define SAFE_DELETE(p) { if(p) delete p; p = NULL; }	// << :
#define SAFE_ADD_REF(p) { if(p) p->AddRef() ; }
// << 2017 05 26 xÆÄÀÏ
#define SAFE_DELETE_ARRAY(p) { if(p) delete [] p; p = NULL; }
// >>

// >> : 
#define SINGLETONE(class_name) \
		private : \
			class_name(void) ;  \
			~class_name(void) ; \
		public :\
			static class_name* GetInstance() \
			{ \
				static class_name instance ; \
				return &instance ; \
			} 

struct ST_PN_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3	n;
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL };
};

struct ST_PC_VERTEX
{
	D3DXVECTOR3 p; 
	D3DCOLOR	c; 
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};

struct ST_PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;
	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
};

struct ST_PNT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2 t;

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
};

struct ST_POS_SAMPLE
{
	int n;
	D3DXVECTOR3 v;

	ST_POS_SAMPLE()
		: n(0), v(0,0,0)
	{
	}
};

struct ST_ROT_SAMPLE
{
	int n;
	D3DXQUATERNION q;

	ST_ROT_SAMPLE()
		: n(0)
	{
		D3DXQuaternionIdentity(&q);
	}
};

struct ST_SPHERE
{
	float fRadius;
	D3DXVECTOR3 vCenter;
	bool isPicked;
	ST_SPHERE() : fRadius(0.0f), vCenter(0, 0, 0) {}
};

struct ST_SIZEN
{
	int nWidth;
	int nHeight;
	ST_SIZEN() : nWidth(0), nHeight(0){}
	ST_SIZEN(int x, int y)
		:nWidth(x), nHeight(y){}
};

enum eNetworkType
{
	E_NONE = 0,
	E_SET_MY_NETWORK_ID,
	E_SET_OTHER_NETWORK_ID,
	E_REFRESH_WAITING_ROOM,
	E_READY,
	E_NETWORK_LOGOUT,
};

struct ST_NETWORK
{
	eNetworkType	eNetType;
	int				nNetID;
	string			sPlayerName;
	bool			isReady = false;
};

struct ST_BONE : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
};

struct ST_BONE_MESH : public D3DXMESHCONTAINER
{
	std::vector<D3DMATERIAL9>		vecMtl;
	std::vector<LPDIRECT3DTEXTURE9> vecTexture;
	LPD3DXMESH		pOrigMesh;
	D3DXMATRIX**	ppBoneMatrixPtrs;
	D3DXMATRIX*		pBoneOffsetMatrices;
	D3DXMATRIX*		pCurrentBoneMatrices;
};

struct ST_TILE_INFO
{
	enum eTYPE
	{
		NONE,
		WALL,
		CHAR,
		MONSTER,
		MONSTER_SPAWN,
		DEST,
		MONSTER_SPAWN_BLOCK,
	};

	enum eAStarType
	{
		EMPTY,
		OPEN,
		CLOSE,
		PATH,
		START,
		END,
	};

	int				idX;
	int				idY;
	float			fG;
	float			fH;
	float			fF;
	eTYPE			type;
	eAStarType		aStarType;
	RECT			rc;
	D3DXVECTOR3		vecCenter;
	ST_TILE_INFO*	pParent = NULL;

	bool			isFNode;

	ST_TILE_INFO() :idX(-1), idY(-1), type(NONE) {}
};

struct ST_OBJECT_INFO
{
	LPD3DXMESH              _pMesh;
	D3DMATERIAL9*           _pMeshMaterials;
	LPDIRECT3DTEXTURE9*     _pMeshTextures;
	DWORD                   _dwNumMaterials;
	float               _fScale;
	ST_OBJECT_INFO() : _fScale(1.0f) {}
};

struct ST_OBJECT
{
	enum eTYPE
	{
		NONE,
		WALL,
		OBJ,
		CHAR,
		MONSTER_SPAWN,
		DEST
	};

	string       sObjName;
	D3DXVECTOR3  vCenter;

	eTYPE       eType;
	float       fRot;
	bool       isDelete;
	int          idX, idY;

	ST_OBJECT() : vCenter(0, 0, 0), fRot(0.0f), idX(-1), idY(-1), eType(NONE), isDelete(false), sObjName("") {}
};

#define NUM_TILE 300

#define WINSTYLE WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN

#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline varType& Get##funName(void) { return varName; }\
public: inline void Set##funName(varType& var){ varName = var; }

#define SYNTHESIZE_ADD_REF(varType, varName, funName)    \
protected: varType varName; \
public: virtual varType Get##funName(void) const { return varName; } \
public: virtual void Set##funName(varType var){\
	if (varName != var) {\
	SAFE_ADD_REF(var);\
	SAFE_RELEASE(varName);\
	varName = var;\
	}\
}
// << :
#include "cDeviceManager.h"
#include "cTextureManager.h"
#include "cFontManager.h"
#include "cTimeManager.h"
#include "cKeyManager.h"
#include "cSoundManager.h"
#include "cObjectManager.h"
#include "cSceneManager.h"
#include "cGameManager.h"
#include "cNetworkManager.h"

#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
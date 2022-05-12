#include "stdafx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "Terrain.h"


CStage::CStage()
{
}


CStage::~CStage()
{
	Release_Scene();
}

HRESULT CStage::Ready_Scene()
{
	//const TCHAR* pFilePath, TEXTYPE eType, const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iCnt = 0

	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Terrain/Tile/Tile%d.png", 
		TEX_MULTI,
		L"Terrain",
		L"Tile",
		12)))
	{
		ERR_MSG(L"Tile Texture Insert Failed");
		return E_FAIL;
	}	


	CObj*		pTerrain = new CTerrain;

	if(nullptr != pTerrain)
		pTerrain->Initialize();

	CObjMgr::Get_Instance()->Add_Object(CObjMgr::TERRAIN, pTerrain);
	
	return S_OK;
}

void CStage::Update_Scene()
{
	CObjMgr::Get_Instance()->Update();
}

void CStage::Late_Update_Scene()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render_Scene()
{
	CObjMgr::Get_Instance()->Render();
}

void CStage::Release_Scene()
{
	
}

CStage* CStage::Create(void)
{
	CStage*		pInstance = new CStage;

	if (FAILED(pInstance->Ready_Scene()))
		return nullptr;

	return pInstance;
}

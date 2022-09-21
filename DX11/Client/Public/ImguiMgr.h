#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "GameInstance.h"

BEGIN(Engine)
class CGameObject;
class CCollider;
class CNavigation;
END

class CImguiMgr final : public CBase
{
	DECLARE_SINGLETON(CImguiMgr)

	
public:
	CImguiMgr();
	virtual ~CImguiMgr() = default;

public:
	virtual void Free() override;

public:
	void Init(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	void Tick(_float fTimeDelta);
	void Render();

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

	bool show_demo_window = true;
	bool show_another_window = false;

	bool show_Map_Tool = false;
	bool show_Object_Tool = false;
	bool show_Collider_Tool = false;
	bool show_Navigation_Tool = false;

private:
	void Set_Prototype();


	void Tool_Map();
	void Tool_Object();
	void Tool_Collider();
	void Tool_Navigation();

	void Translation();
	void Rotation();
	void Scaling();

	void Picking_Object();
	void MoveObject(_float4 _fPosition);
	void CollocateHouse();
	void CollocateObject();
	void CollocateCollider();

	void Save_Map(const char* strStageName,const char* strFileName);
	void Load_Map(const char* strStageName, const char* strFileName);

	void Save_Object(const char* strStageName, const char* strFileName);
	void Load_Object(const char* strStageName, const char* strFileName);

	void Save_Collider(const char* strStageName, const char* strFileName);
	void Load_Collider(const char* strStageName, const char* strFileName);
	void Load_Wall(const char* strStageName, const char* strFileName);

	void Save_Navigation(const char* strStageName, const char* strFileName);
	void Load_Navigation(const char* strStageName, const char* strFileName);

private:

	vector<_float3>		m_vNavigationPoints;
	CNavigation*		m_pNavigationCom;


	vector<CGameObject*> m_vecPrototypeHouse;
	vector<CGameObject*> m_vecPrototypeObject;

	vector<CGameObject*> m_vecCollocatedHouse[(_uint)MODEL_TAG::MODEL_END];

	vector<CGameObject*> m_vecCollocatedObject[(_uint)LAYER::LAYER_END];
	vector<OBJ_TAG>		 m_vecObjectTag[(_uint)LAYER::LAYER_END];
	_int				 m_iSelectedIndex = 0;//오브젝트 선택 시 OBJ_TAG와 순서 맞춰주기

	CGameObject*		m_ColliderPrototype = nullptr;
	vector<CGameObject*> m_vecCollider;

	CGameObject* m_WallPrototype = nullptr;
	vector<CGameObject*> m_vecWall;


	//선택한 오브젝트

	CGameObject* m_pPlayer = nullptr;
	CCollider* m_pRayCom = nullptr;

	CGameObject* m_pSelectedObject = nullptr;
	CTransform* m_pSelectedTransform = nullptr;
	_vector		m_vSelectedOffSet;
	//지형
	//CGameObject* m_pTerrain = nullptr;
	CTransform* m_pTerrainTransform = nullptr;
	CVIBuffer_Terrain* m_pTerrainVIBuffer = nullptr;
};
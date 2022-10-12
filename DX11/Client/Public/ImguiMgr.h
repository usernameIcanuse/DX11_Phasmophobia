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

	_bool show_Map_Tool = false;
	_bool show_Object_Tool = false;
	_bool show_Collider_Tool = false;
	_bool show_Navigation_Tool = false;
	_bool show_Light_Tool = false;
private:
	void Set_Prototype();


	void Tool_Map();
	void Tool_Light();
	void Tool_Object();
	void Tool_Collider();
	void Tool_Navigation();

	void Translation();
	void Rotation();
	void Scaling();

	void Sort_Points_ClockWise();

	void Picking_Light();

	void Picking_Object();
	void MoveObject(_float4 _fPosition);
	void CollocateHouse();
	void CollocateObject();
	void CollocateCollider();

	void Save_Map(const char* strStageName,const char* strFileName);
	void Load_Map(const char* strStageName, const char* strFileName);

	void Save_Light(const char* strStageName, const char* strFileName);
	void Load_Light(const char* strStageName, const char* strFileName);

	void Save_Object(const char* strStageName, const char* strFileName);
	void Load_Object(const char* strStageName, const char* strFileName);

	void Save_Collider(const char* strStageName, const char* strFileName);
	void Load_Collider(const char* strStageName, const char* strFileName);
	void Load_Wall(const char* strStageName, const char* strFileName);

	void Save_Navigation(const char* strStageName, const char* strFileName);
	void Load_Navigation(const char* strStageName, const char* strFileName);

private:

	vector<_float3>				m_vNavigationPoints;
	vector<_float3>				m_vSavePoints;
	vector<_float3>				m_vCellPoints;
	CNavigation*				m_pNavigationCom = nullptr;
	CVIBuffer_Point_Instance* m_pVIBufferPoint = nullptr;


	vector<CGameObject*> m_vecPrototypeHouse;
	vector<CGameObject*> m_vecPrototypeObject;

	vector<CGameObject*> m_vecCollocatedHouse[(_uint)MODEL_TAG::MODEL_END];

	vector<CGameObject*> m_vecCollocatedObject[(_uint)LAYER::LAYER_END];
	vector<OBJ_TAG>		 m_vecObjectTag[(_uint)LAYER::LAYER_END];
	_int				 m_iSelectedIndex = 0;//������Ʈ ���� �� OBJ_TAG�� ���� �����ֱ�

	CGameObject*		m_ColliderPrototype = nullptr;
	vector<CGameObject*> m_vecCollider;

	CGameObject* m_WallPrototype = nullptr;
	vector<CGameObject*> m_vecWall;

	vector<pair<CGameObject*, _int>> m_vecLightSwitch;//����ġ, ����� ���� ����
	vector<CGameObject*>			 m_vecTempBulb;
	vector<CGameObject*>			 m_vecBulb[20];//����ġ(�ε���)�� ������ �ִ� ����
	CGameObject* m_pLightSwitch = nullptr;
	CGameObject* m_pPrototypeSwitch = nullptr;
	CTransform* m_pSwitchTransform = nullptr;
	CGameObject* m_pLightBulb = nullptr;
	CGameObject* m_pPrototypeBulb = nullptr;
	CTransform* m_pBulbTransform = nullptr;
	_int		 m_iSelectSwitchIndex = -1;

	//������ ������Ʈ

	CGameObject* m_pPlayer = nullptr;
	CCollider* m_pRayCom = nullptr;

	CGameObject* m_pSelectedObject = nullptr;
	CTransform* m_pSelectedTransform = nullptr;
	_vector		m_vSelectedOffSet;
	//����
	//CGameObject* m_pTerrain = nullptr;
	CTransform* m_pTerrainTransform = nullptr;
	CVIBuffer_Terrain* m_pTerrainVIBuffer = nullptr;
};
#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "GameInstance.h"

BEGIN(Engine)
class CGameObject;
END

class CImguiMgr final : public CBase
{
	DECLARE_SINGLETON(CImguiMgr)

public:
	/*�����Ҷ� ���� ������ ������*/
	typedef struct tagObjData
	{
		LAYER		tLayerTag;
		OBJ_TAG			tObjTag;
		_matrix		matWorld;
	} OBJ_DATA;

	typedef struct tagHouseData
	{
		MODEL_TAG	tModelTag;
		_matrix		matWorld;
	}MAP_DATA;//layer ������ house


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

private:
	void Set_Prototype();


	void Tool_Map();
	void Tool_Object();

	void Rotation();
	void Scaling();

	void Picking_Object();
	void CollocateHouse();
	void CollocateObject();

	void Save_Map(const char* strStageName,const char* strFileName);
	void Load_Map(const char* strStageName, const char* strFileName);

	void Save_Object(const char* strStageName, const char* strFileName);
	void Load_Object(const char* strStageName, const char* strFileName);

private:
	vector<CGameObject*> m_vecPrototypeHouse;
	vector<CGameObject*> m_vecPrototypeObject;

	vector<CGameObject*> m_vecCollocatedHouse[(_uint)MODEL_TAG::MODEL_END];
	vector<CGameObject*> m_vecCollocatedObject[(_uint)LAYER::LAYER_END];
	vector<OBJ_TAG>		 m_vecObjectTag[(_uint)LAYER::LAYER_END];
	_int				 m_iSelectedIndex = 0;//������Ʈ ���� �� OBJ_TAG�� ���� �����ֱ�

	//������ ������Ʈ
	CGameObject* m_pSelectedObject = nullptr;
	CTransform* m_pSelectedTransform = nullptr;
	//����
	//CGameObject* m_pTerrain = nullptr;
	CTransform* m_pTerrainTransform = nullptr;
	CVIBuffer_Terrain* m_pTerrainVIBuffer = nullptr;
};
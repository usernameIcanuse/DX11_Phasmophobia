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
	/*저장할때 같이 보내줄 데이터*/
	typedef struct tagObjData
	{
		LAYER		tLayerTag;
		OBJ_TAG			tObjTag;
		_matrix		matWorld;
	} OBJ_DATA;


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

private:
	void Set_Prototype();


	void ShowWindow(bool* p_open);
	void Tool_Map();


	void Object_MapTool();//오브젝트 배치
	void Map_MapTool();//갈 수 있는 영역 배치

	void Rotation();
	void Scaling();
	void Picking_Object();
	void CollocateObject();

	void Save(char* strFileName);
	void Load(char* strFileName);


private:
	vector<CGameObject*> m_vecPrototype;
	vector<CGameObject*> m_vecCollocatedObject;
	_int				 m_iSelectedIndex = 0;

	string	m_strSelectObject_Map = "";


	list<string> m_PrototypeTagList;


	string	m_strCurObj = "";


	// 저장할 때
	OBJ_DATA m_tObjData;
	// 설치할 때
	OBJ_DATA m_tInstallObjData;

	//선택한 오브젝트
	CGameObject* m_pSelectedObject = nullptr;
	CTransform* m_pSelectedTransform = nullptr;
	//지형
	//CGameObject* m_pTerrain = nullptr;
	CTransform* m_pTerrainTransform = nullptr;
	CVIBuffer_Terrain* m_pTerrainVIBuffer = nullptr;
};
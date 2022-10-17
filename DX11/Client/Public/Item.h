#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CModel;
class CCollider;
END

BEGIN(Client)

class CItem : public CGameObject
{
public:
	enum ITEM {FLASHLIGHT,UVLIGHT, ITEM_END};
	
protected:
	CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem(const CItem& rhs);
	virtual ~CItem() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void OnEventMessage(const _tchar* pMessage);
	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);
public:
	virtual void Turn_Switch() {};
	virtual _bool Install(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem = nullptr) 
	{
		return false;

	}

	virtual _float3 Get_AdjustPos();
	virtual void	Update_Collider();
	virtual _bool	Keep_Item()
	{
		return true;
	}
	
	virtual void Set_TempModel_Pos(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem = nullptr) {}
	virtual void Frequency_Control(_long lMouseMove) {}

	virtual void MalFunction(_float fTimeDelta = 0.f) {}
	virtual void Normal_Operation(_float fTimeDelta = 0.f) {}

	virtual void Call_EventFunc(_float fTimeDelta = 0.f);
	virtual void Change_Item(_bool _bFlag)
	{
		Set_Enable(_bFlag);
	}
	
	virtual void Adjust_Item(CTransform* _pPlayerTransform);
public:
	void	Set_Install(_bool _bInstall)
	{
		m_bInstalled = _bInstall;
	}
	_int		Get_ItemType()
	{
		return m_eType;
	}

	_bool	Get_Switch()
	{
		return m_bSwitch;
	}


protected:
	CShader*    m_pShaderCom = nullptr;
	CRenderer*  m_pRendererCom = nullptr;
	CModel*     m_pModelCom = nullptr;
	CCollider*  m_pOBBCom = nullptr;

	_bool		m_bSwitch = false;
	_bool		m_bInstalled = false;
	_float3		m_vAdjustpos = _float3(1.f,1.5f,2.f);

	ITEM		m_eType = ITEM_END;

	function<void(CItem*,_float)> m_pEventFunc;
protected:
	virtual	HRESULT	Setup_Component();

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END
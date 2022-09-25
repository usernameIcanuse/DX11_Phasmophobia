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

	virtual void MalFunction() {}
	virtual void Normal_Operation() {}

protected:
	CShader*    m_pShaderCom = nullptr;
	CRenderer*  m_pRendererCom = nullptr;
	CModel*     m_pModelCom = nullptr;
	CCollider*  m_pOBBCom = nullptr;

	_bool		m_bSwitch = false;
	_float3		m_vAdjustpos = _float3(1.f,1.5f,2.f);

	function<void(CItem*)> m_pEventFunc;
protected:
	virtual	HRESULT	Setup_Component();
	virtual HRESULT SetUp_ShaderResource()PURE;

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END
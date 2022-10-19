#pragma once
#include "Item.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CLight;
class CRigidBody;
END


BEGIN(Client)

class CFlashLight final : public CItem
{
private:
	CFlashLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFlashLight(const CFlashLight& rhs);
	virtual ~CFlashLight() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	//virtual void OnEventMessage(const _tchar* pMessage);


public:
	virtual void Turn_Switch()
	{
		m_bSwitch = !m_bSwitch;
	}

	virtual void MalFunction(_float fTimeDelta = 0.f);
	virtual void Normal_Operation(_float fTimeDelta = 0.f);
	virtual void Change_Item(_bool _bFlag) 
	{
		m_bRenderModel = _bFlag;
	} 
	virtual void Adjust_Item(CTransform* _pPlayerTransform);
	virtual void Drop_Item(_vector vPower);
private:
	
	CLight* m_pSpotLight = nullptr;
	CTransform* m_pAdjustTransform = nullptr;

	_bool	m_bRenderModel= true;
	
	_float4	m_vDiffuse;

	_float4 m_vBlinkDiffuse;
	_float m_fBlinkTime = 0.f;

private:
	virtual	HRESULT	Setup_Component() override;
	HRESULT	Setup_Light();


public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);


public:
	static CFlashLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
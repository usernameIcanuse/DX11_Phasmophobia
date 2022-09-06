#pragma once
#include "Item.h"
#include "Client_Defines.h"



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

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);


private:
	virtual	HRESULT	Setup_Component() override;
	virtual HRESULT SetUp_ShaderResource() override;
public:
	static CFlashLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
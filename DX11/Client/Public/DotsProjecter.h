#pragma once
#include "Item.h"
#include "Client_Defines.h"




BEGIN(Client)

class CDotsProjecter final : public CItem
{
private:
	CDotsProjecter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDotsProjecter(const CDotsProjecter& rhs);
	virtual ~CDotsProjecter() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual _bool Install(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook) { return true; };


public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

private:
	virtual	HRESULT	Setup_Component() override;
	virtual HRESULT SetUp_ShaderResource() override;

public:
	static CDotsProjecter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
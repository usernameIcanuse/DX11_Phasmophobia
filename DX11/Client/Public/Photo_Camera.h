#pragma once
#include "Item.h"
#include "Client_Defines.h"

BEGIN(Client)

class CPhoto_Camera final: public CItem
{
private:
	CPhoto_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPhoto_Camera(const CPhoto_Camera& rhs);
	virtual ~CPhoto_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	//virtual void OnEventMessage(const _tchar* pMessage);
public:
	virtual _bool Install(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook);

	virtual void MalFunction(_float fTimeDelta = 0.f) {}
	virtual void Normal_Operation(_float fTimeDelta = 0.f) {}


public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

private:
	virtual	HRESULT	Setup_Component() override;

public:
	static CPhoto_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
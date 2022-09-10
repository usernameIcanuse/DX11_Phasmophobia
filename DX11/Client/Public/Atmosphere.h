#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CRenderer;
class CCollider;
END

BEGIN(Client)

class CAtmosphere final: public CGameObject
{
protected:
	CAtmosphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAtmosphere(const CAtmosphere& rhs);
	virtual ~CAtmosphere() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


private:
	virtual	HRESULT	Setup_Component();
	virtual HRESULT SetUp_ShaderResource();

private:
	CCollider*		 m_pOBBCom = nullptr;//COLLISION_TYPE::ATMOSPHERE

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

public:
	static CAtmosphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
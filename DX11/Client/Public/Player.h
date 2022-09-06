#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
END


BEGIN(Client)

class CInventory;

class CPlayer : public CGameObject
{
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


private:
	CCollider* m_pRayCom = nullptr;
	CCollider* m_pOBBCom = nullptr;

	CInventory* m_pInventory = nullptr;

	_float			m_fDist = FLT_MAX;
	_bool			m_bFlag = false;
	CGameObject*	m_pItem = nullptr;
	_float3			m_vColliderPos;


private:
	HRESULT	Setup_Component();
	HRESULT Setup_Camera();
	HRESULT Setup_Inventory();

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END

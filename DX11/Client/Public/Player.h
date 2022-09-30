#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
class CNavigation;
class CRenderer;

END


BEGIN(Client)

class CInventory;

class CPlayer final: public CGameObject
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

public:
#ifdef _DEBUG
	CNavigation* Get_Navigation()
	{
		return m_pNavigationCom;
	}
#endif

private:
	CCollider* m_pRayCom = nullptr;
	CCollider* m_pOBBCom = nullptr;
	CNavigation* m_pNavigationCom = nullptr;

#ifdef _DEBUG
	CRenderer*   m_pRendererCom = nullptr;
#endif

	CInventory* m_pInventory = nullptr;

	_float			m_fDist = FLT_MAX;
	CGameObject*	m_pItem = nullptr;
	class CDoor*	m_pDoor = nullptr;

	_float3			m_vColliderPos;
	COLLISION_TYPE	m_eColliderType = COLLISION_TYPE::TYPE_END;
	_float4			m_vColliderLook;
	
	_bool			m_bGrab = false;
	CGameObject*	 m_pTripod = nullptr;


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

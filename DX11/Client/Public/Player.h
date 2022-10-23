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
	_bool	Picking_Navigation(RAY tMouseRay, _float4& vPickedPos);
	_int	Get_CurrentIndex();
	_bool*	IsInHouse()
	{
		return &m_bIsInHouse;
	}
#ifdef _DEBUG
	CNavigation* Get_Navigation()
	{
		return m_pCurrNavigation;
	}
#endif

private:
	CCollider* m_pAABBCom = nullptr;
	CNavigation* m_pNaviHouseCom = nullptr;
	CNavigation* m_pNaviOutSideCom = nullptr;

	CNavigation* m_pCurrNavigation = nullptr;

#ifdef _DEBUG
	CRenderer*   m_pRendererCom = nullptr;
#endif

	CInventory* m_pInventory = nullptr;
	class CJournal* m_pJournal = nullptr;
	_bool		m_bIsInHouse = false;

	_bool			m_bLockCursor = true;

private:
	HRESULT	Setup_Component();
	HRESULT Setup_Camera();
	HRESULT Setup_Inventory();
	HRESULT Setup_Journal();

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

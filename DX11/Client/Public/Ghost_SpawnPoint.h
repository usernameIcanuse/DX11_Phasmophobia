#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
class CRenderer;
END


BEGIN(Client)

class CGhost;
/*main*/

class CGhost_SpawnPoint final: public CGameObject
{
private:
	CGhost_SpawnPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGhost_SpawnPoint(const CGhost_SpawnPoint& rhs);
	virtual ~CGhost_SpawnPoint() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	/*area에서 받아와야 할 귀신 상호작용
	emf/dots/spiritbox*/


public:
	_uint	Get_Anger()
	{
		//return m_iAnger;
		return 6;//임시로
	}

private:
	CCollider*		m_pAreaCom = nullptr;
	CCollider*		m_pSpawnPointCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;

	CGhost*			m_pGhost = nullptr;

	_uint	   m_iAnger = 0;
	
private:
	HRESULT	Setup_Component();
	HRESULT Setup_Ghost();

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

public:
	static CGhost_SpawnPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END

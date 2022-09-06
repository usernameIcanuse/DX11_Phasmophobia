#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
END


BEGIN(Client)



class CGhost : public CGameObject
{
private:
	CGhost(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGhost(const CGhost& rhs);
	virtual ~CGhost() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	 

private:
	CCollider* m_pOBBCom = nullptr;
	
	
private:
	HRESULT	Setup_Component();
	HRESULT Setup_Camera();
	HRESULT Setup_Inventory();

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

public:
	static CGhost* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END

#pragma once
#include "Item.h"
#include "Client_Defines.h"

BEGIN(Client)

class CVideo_Camera;

class CTripod final : public CItem
{
private:
	CTripod(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTripod(const CTripod& rhs);
	virtual ~CTripod() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void	Update_Collider();
	virtual _bool   Keep_Item()
	{
		return false;
	}
	

public:
	void    Connect_Camera(CVideo_Camera* pCamera)
	{
		m_pConnectedCamera = pCamera;
	}
	void	Seperate_Camera()
	{
		m_pConnectedCamera = nullptr;
	}

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

private:
	CVideo_Camera* m_pConnectedCamera = nullptr;
	CCollider*	   m_pTripodCom = nullptr;

private:
	virtual	HRESULT	Setup_Component() override;
	virtual HRESULT SetUp_ShaderResource() override;
public:
	static CTripod* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
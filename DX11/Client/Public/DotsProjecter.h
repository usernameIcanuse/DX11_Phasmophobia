#pragma once
#include "Item.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
class CLight;
END

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


	//virtual void OnEventMessage(const _tchar* pMessage);

public:
	virtual _bool Install(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem = nullptr);
	
	virtual void Set_TempModel_Pos(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem = nullptr);
	
	virtual void MalFunction(_float fTimeDelta = 0.f);
	virtual void Normal_Operation(_float fTimeDelta = 0.f);

private:
	CLight*		m_pLight = nullptr;//설치 시 불 켜짐
	CCollider* m_pAreaCom = nullptr;

	CItem* m_pTempDotsModel = nullptr;

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

private:
	virtual	HRESULT	Setup_Component() override;
	HRESULT			Setup_TempModel();
	HRESULT			Setup_Light();

public:
	static CDotsProjecter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#pragma once
#include "Item.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
class CLight;
END

BEGIN(Client)

class CTrailCam final : public CItem
{
private:
	CTrailCam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrailCam(const CTrailCam& rhs);
	virtual ~CTrailCam() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual _bool Install(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem = nullptr);
	virtual void Set_TempModel_Pos(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem = nullptr);

public:
	void Sensor_Activating(_bool _bSwitch);

private:
	CCollider* m_pAreaCom = nullptr; //모션 인식
	CLight* m_pLight  =  nullptr;//인식시 불 켜짐
	CItem* m_pTempTrailCamModel = nullptr;

	CNavigation* m_pNaviHouseCom = nullptr;
	CNavigation* m_pNaviOutSideCom = nullptr;

	CNavigation* m_pCurrNavigation = nullptr;

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

private:
	virtual	HRESULT	Setup_Component() override;
	HRESULT			Setup_TempModel();
	HRESULT			Setup_Light();
public:
	static CTrailCam* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
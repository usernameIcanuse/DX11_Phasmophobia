#pragma once
#include "Item.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CTempTrailCam final : public CItem
{
private:
	CTempTrailCam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTempTrailCam(const CTempTrailCam& rhs);
	virtual ~CTempTrailCam() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void Set_TempModel_Pos(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem = nullptr);


private:
	virtual	HRESULT	Setup_Component() override;

public:
	static CTempTrailCam* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
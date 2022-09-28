#pragma once
#include "Item.h"
#include "Client_Defines.h"

BEGIN(Client)

class CTempNote final : public CItem
{
private:
	CTempNote(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTempNote(const CTempNote& rhs);
	virtual ~CTempNote() = default;

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
	static CTempNote* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
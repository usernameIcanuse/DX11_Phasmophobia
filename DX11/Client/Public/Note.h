#pragma once
#include "Item.h"
#include "Client_Defines.h"

BEGIN(Client)

class CNote final : public CItem
{
private:
	CNote(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNote(const CNote& rhs);
	virtual ~CNote() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual	HRESULT	Setup_Component() override;
	virtual HRESULT SetUp_ShaderResource() override;
public:
	static CNote* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
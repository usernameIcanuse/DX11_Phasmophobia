#pragma once
#include "Item.h"
#include "Client_Defines.h"

BEGIN(Client)

class CVideo_Camera final: public CItem
{
private:
	CVideo_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVideo_Camera(const CVideo_Camera& rhs);
	virtual ~CVideo_Camera() = default;

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
	static CVideo_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
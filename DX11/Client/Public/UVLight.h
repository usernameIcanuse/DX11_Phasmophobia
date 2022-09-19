#pragma once
#include "Item.h"
#include "Client_Defines.h"

BEGIN(Client)

class CUVLight final : public CItem
{
private:
	CUVLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUVLight(const CUVLight& rhs);
	virtual ~CUVLight() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void OnEventMessage(const _tchar* pMessage);


public:
	virtual void Turn_Switch()
	{
		m_bSwitch = !m_bSwitch;
	}
	virtual void MalFunction() {}
	virtual void Normal_Operation() {}

	
private:
	virtual	HRESULT	Setup_Component() override;
	virtual HRESULT SetUp_ShaderResource() override;
public:
	static CUVLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#pragma once
#include "Camera.h"
#include "Client_Defines.h"

BEGIN(Client)

class CCamera_Fixed final : public CCamera
{
private:
	CCamera_Fixed(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Fixed(const CCamera_Fixed& rhs);
	virtual ~CCamera_Fixed() = default;

public:

	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;

public:
	void	Set_Target(CTransform* _pTarget);

private:
	CTransform* m_pTarget = nullptr;

public:
	static CCamera_Fixed* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
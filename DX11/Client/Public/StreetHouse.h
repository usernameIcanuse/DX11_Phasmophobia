#pragma once
#include "House.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CRenderer;
class CModel;

END

BEGIN(Client)

class CStreetHouse final : public CHouse
{
private:
	CStreetHouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStreetHouse(const CStreetHouse& rhs);
	virtual ~CStreetHouse() = default;

public:
	virtual HRESULT Initialize(void* pArg);

private:
	virtual	HRESULT	Setup_Component();

public:
	static CStreetHouse* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
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

class CTutorial final : public CHouse
{
private:
	CTutorial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTutorial(const CTutorial& rhs);
	virtual ~CTutorial() = default;

public:
	virtual HRESULT Initialize(void* pArg);

private:
	virtual	HRESULT	Setup_Component();

public:
	static CTutorial* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
#pragma once
#include "Ghost.h"
#include "Client_Defines.h"



BEGIN(Client)


class CGoryo final : public CGhost
{
private:
	CGoryo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGoryo(const CGoryo& rhs);
	virtual ~CGoryo() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	static CGoryo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END

#pragma once
#include "Ghost.h"
#include "Client_Defines.h"



BEGIN(Client)


class COni final : public CGhost
{
private:
	COni(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	COni(const COni& rhs);
	virtual ~COni() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	static COni* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END

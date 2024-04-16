#pragma once
#include "Ghost.h"
#include "Client_Defines.h"



BEGIN(Client)


class CWraith final : public CGhost
{
private:
	CWraith(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWraith(const CWraith& rhs);
	virtual ~CWraith() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	static CWraith* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END

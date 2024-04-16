#pragma once
#include "Ghost.h"
#include "Client_Defines.h"



BEGIN(Client)


class CJinn final: public CGhost
{
private:
	CJinn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CJinn(const CJinn& rhs);
	virtual ~CJinn() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	static CJinn* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END

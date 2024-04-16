#pragma once
#include "Ghost.h"
#include "Client_Defines.h"



BEGIN(Client)


class CPhantom final : public CGhost
{
private:
	CPhantom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPhantom(const CPhantom& rhs);
	virtual ~CPhantom() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	static CPhantom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END

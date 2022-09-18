#pragma once
#include "GameObject.h"
#include "Client_Defines.h"


BEGIN(Client)

class CGhost_SpawnPoint;


class CGhost_Status final: public CGameObject
{
	friend class CGhost_SpawnPoint;
private:
	CGhost_Status(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGhost_Status(const CGhost_Status& rhs);
	virtual ~CGhost_Status() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_uint	   m_iAggression = 0;
	_uint	   m_iEMF = 1;


public:
	static CGhost_Status* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;//SpawnPoint∏¶ ≥—∞‹¡‹
	virtual void Free() override;

};

END

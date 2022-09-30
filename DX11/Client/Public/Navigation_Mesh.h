#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CNavigaion;
END

BEGIN(Client)

class CNavigation_Mesh final : public CGameObject
{
private:
	CNavigation_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation_Mesh(const CNavigation_Mesh& rhs);
	virtual ~CNavigation_Mesh() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CNavigation*		    m_pNavigationCom = nullptr;

#ifdef _DEBUG
	CRenderer*				m_pRendererCom = nullptr;		
#endif

private:
	HRESULT SetUp_Components();

public:
	static CNavigation_Mesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
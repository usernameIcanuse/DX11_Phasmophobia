#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
class CRenderer;
END

BEGIN(Client)

class CWall_Collider final : public CGameObject
{
private:
	CWall_Collider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWall_Collider(const CWall_Collider& rhs);
	virtual ~CWall_Collider() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


private:
	CCollider* m_pOBBCom = nullptr;
	CCollider* m_pOBBDirCom = nullptr;//lookπÊ«‚ æÀ∑¡¡‹
	CRenderer* m_pRendererCom = nullptr;


private:
	virtual	HRESULT	Setup_Component();



public:
	static CWall_Collider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
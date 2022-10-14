#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
#ifdef _DEBUG
class CRenderer;
#endif
class CNavigation;
END


BEGIN(Client)

class CGhost_SpawnPoint;


class CGhost_Behavior final: public CGameObject
{
private:
	CGhost_Behavior(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGhost_Behavior(const CGhost_Behavior& rhs);
	virtual ~CGhost_Behavior() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void Call_EventFunc(_float fTimeDelta = 0.f);
	virtual void OnEventMessage(const _tchar* pMessage);

public:
	void Normal_Operation(_float fTimeDelta);
	void Event(_float fTimeDelta);
	void Attack(_float fTimeDelta);

public:
	void Set_Owner(CTransform* _pOwnerTransform)
	{
		m_pOwnerTransform = _pOwnerTransform;
		Safe_AddRef(m_pOwnerTransform);
	}
	void Set_NaviIndex(_int iCurrentIndex);

private:
	CTransform* m_pOwnerTransform = nullptr;
	/*플레이어도 미리 가지고 있음?*/
	CTransform* m_pPlayerTransform = nullptr;

	CNavigation* m_pNavigationCom = nullptr;

	function<void(CGhost_Behavior*, _float)> m_pEventFunc;

	_float		m_fChangeDir = 0.f;
	_float		m_fRadian = 0.f;
	_float		m_fIdleTime = 3.f;

#ifdef _DEBUG
	CRenderer* m_pRendererCom = nullptr;
#endif
private:
	HRESULT Setup_Component(_int iNaviIndex);

public:
	static CGhost_Behavior* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;//SpawnPoint를 넘겨줌
	virtual void Free() override;

};

END

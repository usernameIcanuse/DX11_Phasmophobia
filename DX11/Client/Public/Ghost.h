#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
class CModel;
class CRenderer;
class CNavigation;
class CShader;
END


BEGIN(Client)

class CGhost_SpawnPoint;


class CGhost final: public CGameObject
{
	friend class CGhost_SpawnPoint;
private:
	CGhost(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGhost(const CGhost& rhs);
	virtual ~CGhost() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);

	virtual void Set_Enable(_bool _bEnable);
	virtual void OnEventMessage(const _tchar* pMessage);
	virtual void Call_EventFunc(_float fTimeDelta = 0.f);
	
public:
	_bool	Check_GhostWriting();
	
private:
	void Stop_Updating_SpawnPoint(); //스폰포인트 고정

private: /*기본 귀신 상호작용, 기능*/
	void Light_Attack(_float fTimeDelta =0.f); //이벤트
	void Attack(_float fTimeDelta = 0.f); //헌팅
	void Normal_Operation(_float fTimeDelta = 0.f);


private:
	CCollider*		m_pOBBCom = nullptr; // attack
	CCollider*		m_pGhostCom = nullptr;  //ghost
	CModel*			m_pModelCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;

#ifdef _DEBUG
	_tchar		m_szEvent[MAX_PATH] = TEXT("");//이벤트 출력
#endif

	CGhost_SpawnPoint* m_pSpawnPoint = nullptr;
	_float				m_fUpdatePointTime = 0.f;

	//ghost가 핸드 프린트 하나를 가지고 있고 시간 지나서 문 부딪힐 때마다 재활용
	
	function<void(CGhost*,_float)>	m_EventFunc;

	_float				m_fTime = 0.f;

	_float				m_fEventTime = 10.f;
	_float				m_fAttackTime = 10.f;

	_bool				m_bGhostWriting = false;
	_bool				m_bHandPrint = true;
	_bool				m_bDotsProjecter = false;

	_bool				m_bCheckGhostWriting = false;
	_bool				m_bCheckHandPrint = false;
	_bool				m_bCheckDotsProjecter = false;

	_float				m_fIdleTime = 3.f;
	_float				m_fHandPrintCoolTime = -1.f;

	
private:
	HRESULT	Setup_Component();
	HRESULT Setup_SpawnPoint();
	HRESULT Setup_Bahavior();

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

public:
	static CGhost* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END

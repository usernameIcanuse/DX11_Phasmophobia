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
class CGhost_Behavior;

class CGhost final: public CGameObject
{
	friend class CGhost_SpawnPoint;
public:


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


private: /*�⺻ �ͽ� ��ȣ�ۿ�, ���*/
	void Light_Attack(_float fTimeDelta =0.f); //�̺�Ʈ
	void Attack(_float fTimeDelta = 0.f); //����
	void Normal_Operation(_float fTimeDelta = 0.f);

	void DotsProjecter(_float fTimeDelta);

	void Move_To_SpawnPoint(_vector _pPosition);


private:
	CCollider*		m_pOBBCom = nullptr; // attack

	CModel*			m_pModelCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;

	CGhost_Behavior* m_pBehavior = nullptr;

	_int			m_iPassIndex = 0;

#ifdef _DEBUG
	_tchar		m_szEvent[MAX_PATH] = TEXT("");//�̺�Ʈ ���
#endif
	CGhost_SpawnPoint* m_pSpawnPoint = nullptr;
	_float				m_fUpdatePointTime = 1.f;

	//ghost�� �ڵ� ����Ʈ �ϳ��� ������ �ְ� �ð� ������ �� �ε��� ������ ��Ȱ��
	
	function<void(CGhost*,_float)>	m_EventFunc;

	_float				m_fTime = 0.f;

	_float				m_fEventTime = 10.f;
	_float				m_fAttackTime = 10.f;

	_float				m_fIdleTime = 0.f;

	_float				m_fRenderModel = 0.f;
	_float				m_fHideModel = 0.f;


private:
	HRESULT	Setup_Component();
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

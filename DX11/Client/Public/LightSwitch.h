#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CModel;
class CCollider;
class CLight;

END

BEGIN(Client)

class CLightSwitch final : public CGameObject
{
private:
	CLightSwitch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLightSwitch(const CLightSwitch& rhs);
	virtual ~CLightSwitch() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void OnEventMessage(const _tchar* pMessage);
	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);

public:
	void Turn_Switch(_bool _bSwitch);

public:
	HRESULT	SetUp_ModelCom(const _tchar* pPrototypeTag);

private:
	CShader* m_pShaderCom = nullptr;
	//CTexture* m_pTextureCom = nullptr;
	//CRenderer* m_pRendererCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CCollider* m_pOBBCom = nullptr;

	vector<class CLightBulb*> m_LightBulbList;
	
	_bool	m_bSwitch = false;

/*
	light switch에서 Lightbulb를 만들어줌 그러면 불러올 때 그 자기 위치와 lightbulb위치,light위치 
	함께 받아와서 저장
*/

private:
	virtual	HRESULT	Setup_Component();
	HRESULT		Setup_LightBulb(SWITCH_DATA _tData);

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);


public:
	static CLightSwitch* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CCollider;
class CRenderer;
class CModel;

END

BEGIN(Client)

class CDoor final : public CGameObject
{
private:
	CDoor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDoor(const CDoor& rhs);
	virtual ~CDoor() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void OnEventMessage(const _tchar* pMessage);
	

public:
	void Grab_Door(CGameObject* _pPlayer)
	{
		m_pPlayer = _pPlayer;
	}


public:
	HRESULT	SetUp_ModelCom(const _tchar* pPrototypeTag);

private:
	void Close_Door(_float fTimeDelta = 0.f);

private:
	CShader* m_pShaderCom = nullptr;
	//CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CModel* m_pModelCom = nullptr;

	CCollider* m_pOBBCom = nullptr;

private:
	CGameObject*	m_pPlayer = nullptr;

	_float			m_fRadian = 0.f;
	_bool			m_bMainDoor = false;
	_bool			m_bLock = false;

	function<void(CDoor*, _float)> m_pEventFunc;

private:
	virtual	HRESULT	Setup_Component();
	virtual HRESULT SetUp_ShaderResource();


public:
	static CDoor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
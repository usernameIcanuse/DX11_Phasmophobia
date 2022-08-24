#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CHouse : public CGameObject
{
protected:
	CHouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHouse(const CHouse& rhs);
	virtual ~CHouse() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	CShader* m_pShaderCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CModel* m_pModelCom = nullptr;

protected:
	virtual	HRESULT	Setup_Component();
	virtual HRESULT SetUp_ShaderResource();


public:
	static CHouse* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
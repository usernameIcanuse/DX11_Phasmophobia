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

class CHouse final : public CGameObject
{
private:
	CHouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHouse(const CHouse& rhs);
	virtual ~CHouse() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);

public:
	HRESULT	SetUp_ModelCom(const _tchar* pPrototypeTag );

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CModel* m_pModelCom = nullptr;


private:
	virtual	HRESULT	Setup_Component();



public:
	static CHouse* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
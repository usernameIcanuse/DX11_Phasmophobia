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

class CTruck final : public CGameObject
{
private:
	CTruck(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTruck(const CTruck& rhs);
	virtual ~CTruck() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CModel* m_pModelCom = nullptr;


private:
	virtual	HRESULT	Setup_Component();
	virtual HRESULT SetUp_ShaderResource();


public:
	static CTruck* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
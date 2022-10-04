#pragma once

/* ȭ�鿡 �׷������� ��ü���� �׸��� ������� ��Ƽ� �����ϳ�.  */
/* ������ �ִ� ��ü���� �����Լ��� ȣ�����ֳ�. */

#include "Renderer.h"

BEGIN(Engine)

class CRenderTarget;

class ENGINE_DLL CCamera_Renderer final : public CRenderer
{
private:
	CCamera_Renderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCamera_Renderer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	ID3D11ShaderResourceView* Get_SRV();

	virtual void	Clear_RenderTarget() override;


private:
	virtual HRESULT Begin_RenderTarget(const _tchar* _pMRTTag) override;
	virtual HRESULT End_RenderTarget() override;

	virtual void Set_Environment();
	virtual void End_Environment();

private:
	CRenderTarget* m_pCameraScreen = nullptr;
	class CTransform* m_pOwnerTransform = nullptr;

public:
	static CCamera_Renderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
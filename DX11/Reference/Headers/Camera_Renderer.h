#pragma once

/* ȭ�鿡 �׷������� ��ü���� �׸��� ������� ��Ƽ� �����ϳ�.  */
/* ������ �ִ� ��ü���� �����Լ��� ȣ�����ֳ�. */

#include "Renderer.h"

BEGIN(Engine)

class CRenderTarget;
class CTransform;

class ENGINE_DLL CCamera_Renderer final : public CRenderer
{
private:
	CCamera_Renderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCamera_Renderer() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;

public:
	ID3D11ShaderResourceView* Get_SRV();

private:
	CRenderTarget* m_pCameraScreen = nullptr;
	CTransform* m_pTransformCom = nullptr;
public:
	static CCamera_Renderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
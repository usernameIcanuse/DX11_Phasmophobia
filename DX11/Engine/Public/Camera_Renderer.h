#pragma once

/* 화면에 그려져야할 객체들을 그리는 순서대로 모아서 관리하낟.  */
/* 가지고 있는 객체들의 렌더함수를 호출해주낟. */

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
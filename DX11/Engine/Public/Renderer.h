#pragma once

/* 화면에 그려져야할 객체들을 그리는 순서대로 모아서 관리하낟.  */
/* 가지고 있는 객체들의 렌더함수를 호출해주낟. */

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_TERRAIN,RENDER_NONALPHABLEND, RENDER_ALPHABLEND, RENDER_UI, RENDER_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;
	 
public:
	HRESULT Add_RenderGroup(RENDERGROUP	eGroup, class CGameObject* pGameObject);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Draw_RenderGroup();

private:
	list<class CGameObject*>				m_RenderObjects[RENDER_END];
	typedef list<class CGameObject*>		RENDEROBJECTS;

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
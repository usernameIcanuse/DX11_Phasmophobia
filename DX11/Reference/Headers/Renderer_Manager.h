#pragma once
#include "Base.h"

BEGIN(Engine)

class CRenderer;

class CRenderer_Manager final : public CBase
{
public:
	enum RENDERER_TYPE {VIDEO_CAMERA, PHOTO_CAMERA, RENDERER_STATIC, RENDERER_END};
	DECLARE_SINGLETON(CRenderer_Manager)
public:
	CRenderer_Manager();
	virtual ~CRenderer_Manager() = default;

public:
	HRESULT Add_Renderer(_uint eListIndex,CRenderer* pRenderer);
	HRESULT Draw_RenderGroup();
	void Clear_RendererIndex(_uint eListIndex);
private:
	list<CRenderer*>		m_RendererList[RENDERER_END];

public:
	virtual void Free() override;
};

END
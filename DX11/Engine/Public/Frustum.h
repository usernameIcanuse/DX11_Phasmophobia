#pragma once

#include "Base.h"
#include "Transform.h"
#include "Renderer.h"
BEGIN(Engine)
class CGameObject;

class CFrustum final : public CBase
{
	DECLARE_SINGLETON(CFrustum)

public:
	enum FRUSTUM_TYPE {FRUSTUM_STATIC, FRUSTUM_ITEM,FRUSTUM_END};

	typedef struct
	{
		_float4				m_vWorldPoints[8];
		_float4				m_vWorldPlane[6];
		_float4				m_vLocalPlane[6];
	}FRUSTUMPLANE;
private:
	CFrustum();
	virtual ~CFrustum() = default;
public:
	/* ������������ ����ü�� �����ϴ� ������ ���� ��� ���Ѵ�. */	
	HRESULT Initialize();

	/*���� �������� ������ ����� ���Ѵ�. */
	void Update();
	void Late_Update();

	void Clear_Frustum();
	void Add_ItemFrustum(FRUSTUM_TYPE eFrustumType, CRenderer* _pRenderer, CTransform* _pTransform = nullptr);
	void Add_Object_For_Culling(CGameObject* pGameObject, CRenderer::RENDERGROUP eRenderGroup);

public:
	_bool isIn_Frustum_InLocalSpace(_fvector vLocalPoint, _float fRange, CRenderer::RENDERGROUP eRenderGroup, CGameObject* pGameObject);
	void Transform_ToLocalSpace(_fmatrix WorldMatrixInv);

private:
	void Default_Update();
	void Item_Update();

	_bool	View_Frustum_Culling(_fvector vWorldPoint, _float fRange );
	void	Item_Frustum_Culling(_fvector vWorldPoint, _float fRange, CRenderer::RENDERGROUP eRenderGroup, CGameObject* pGameObject);

private:
	_float4				m_vOriginalPoints[8];
	//////////�� ī�޶� �������� �������� ����ü)/////////////
	///Default
	//_float4				m_vWorldPoints[8];
	//_float4				m_vWorldPlane[6];
	//_float4				m_vLocalPlane[6];//���� �ø�(LOD)������



	list<pair<CTransform*, CRenderer*>> m_listFrustumDesc[FRUSTUM_END];
	vector<FRUSTUMPLANE>		m_FrustumPlane[FRUSTUM_END];
	vector<pair<CGameObject*, CRenderer::RENDERGROUP>>			m_vecObjectList;


public:
	virtual void Free() override;
};

END
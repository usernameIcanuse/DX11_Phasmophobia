#include "..\Public\Frustum.h"
#include "PipeLine.h"
#include "Transform.h"
#include "Renderer.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CFrustum)

CFrustum::CFrustum()
{
}

HRESULT CFrustum::Initialize()
{
	m_vOriginalPoints[0] = _float4(-1.f, 1.f, 0.f, 1.f);
	m_vOriginalPoints[1] = _float4(1.f, 1.f, 0.f, 1.f);
	m_vOriginalPoints[2] = _float4(1.f, -1.f, 0.f, 1.f);
	m_vOriginalPoints[3] = _float4(-1.f, -1.f, 0.f, 1.f);

	m_vOriginalPoints[4] = _float4(-1.f, 1.f, 1.f, 1.f);
	m_vOriginalPoints[5] = _float4(1.f, 1.f, 1.f, 1.f);
	m_vOriginalPoints[6] = _float4(1.f, -1.f, 1.f, 1.f);
	m_vOriginalPoints[7] = _float4(-1.f, -1.f, 1.f, 1.f);

	return S_OK;
}

void CFrustum::Update()
{
	Default_Update();
	Item_Update();	
}

void CFrustum::Late_Update()
{
	if (m_vecObjectList.empty() || m_listFrustumDesc[FRUSTUM_STATIC].empty())
		return;

	for (auto& elem : m_vecObjectList)
	{
		/*충돌체로 절두체 컬링*/
		CTransform* pTransform = (CTransform*)elem.first->Get_Component(CGameObject::m_pTransformTag);
		_vector vWorldPoint = pTransform->Get_State(CTransform::STATE_TRANSLATION);
		if (View_Frustum_Culling(vWorldPoint, 5.f))
		{
			m_listFrustumDesc[FRUSTUM_STATIC].front().second->Add_RenderGroup(elem.second, elem.first);
		}
		Item_Frustum_Culling(vWorldPoint, 5.f, elem.second, elem.first);
	
		Safe_Release(elem.first);
	}
	m_vecObjectList.clear();
}

void CFrustum::Add_ItemFrustum(FRUSTUM_TYPE eFrustumType, CRenderer* _pRenderer, CTransform* _pTransform)
{
	m_listFrustumDesc[eFrustumType].push_back(make_pair(_pTransform, _pRenderer));
	Safe_AddRef(_pTransform);
	Safe_AddRef(_pRenderer);
}

void CFrustum::Add_Object_For_Culling(CGameObject* pGameObject, CRenderer::RENDERGROUP eRenderGroup)
{
	m_vecObjectList.push_back(make_pair(pGameObject, eRenderGroup));
	Safe_AddRef(pGameObject);
}

void CFrustum::Clear_Frustum()
{
	for (auto& elem : m_listFrustumDesc[FRUSTUM_ITEM])
	{
		Safe_Release(elem.first);
		Safe_Release(elem.second);
	}
	m_listFrustumDesc[FRUSTUM_ITEM].clear();

	m_FrustumPlane[FRUSTUM_STATIC].clear();
	m_FrustumPlane[FRUSTUM_ITEM].clear();
}


_bool CFrustum::isIn_Frustum_InLocalSpace(_fvector vLocalPoint, _float fRange, CRenderer::RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	/*for (_uint i = 0; i < 6; ++i)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vLocalPlane[i]), vLocalPoint)))
		{
			return false;
		}		
	}*/

	return true;
}

void CFrustum::Transform_ToLocalSpace(_fmatrix WorldMatrixInv)
{
	_vector		vLocalPoints[8];

	/* 여덟개 정점을 모두 로컬 스페이스로 변경했다. */
	for (_uint i = 0; i < 8; ++i)
		vLocalPoints[i] = XMVector3TransformCoord(XMLoadFloat4(&m_FrustumPlane[FRUSTUM_STATIC].front().m_vWorldPoints[i]), WorldMatrixInv);


	/* 로컬 스페이스 상의 평면 여섯개룰 만드낟. */

	XMStoreFloat4(&m_FrustumPlane[FRUSTUM_STATIC].front().m_vLocalPlane[0], XMPlaneFromPoints(vLocalPoints[1], vLocalPoints[5], vLocalPoints[6]));
	XMStoreFloat4(&m_FrustumPlane[FRUSTUM_STATIC].front().m_vLocalPlane[1], XMPlaneFromPoints(vLocalPoints[4], vLocalPoints[0], vLocalPoints[3]));
	XMStoreFloat4(&m_FrustumPlane[FRUSTUM_STATIC].front().m_vLocalPlane[2], XMPlaneFromPoints(vLocalPoints[4], vLocalPoints[5], vLocalPoints[1]));
	XMStoreFloat4(&m_FrustumPlane[FRUSTUM_STATIC].front().m_vLocalPlane[3], XMPlaneFromPoints(vLocalPoints[3], vLocalPoints[2], vLocalPoints[6]));
	XMStoreFloat4(&m_FrustumPlane[FRUSTUM_STATIC].front().m_vLocalPlane[4], XMPlaneFromPoints(vLocalPoints[5], vLocalPoints[4], vLocalPoints[7]));
	XMStoreFloat4(&m_FrustumPlane[FRUSTUM_STATIC].front().m_vLocalPlane[5], XMPlaneFromPoints(vLocalPoints[0], vLocalPoints[1], vLocalPoints[2]));
}

void CFrustum::Default_Update()
{
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix			ViewMatrixInv = XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW));
	_matrix			ProjMatrixInv = XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ));

	RELEASE_INSTANCE(CPipeLine);

	FRUSTUMPLANE	FrustumPlane;

	for (_uint i = 0; i < 8; ++i)
	{
		XMStoreFloat4(&FrustumPlane.m_vWorldPoints[i], XMVector3TransformCoord(XMLoadFloat4(&m_vOriginalPoints[i]), ProjMatrixInv));
		XMStoreFloat4(&FrustumPlane.m_vWorldPoints[i], XMVector3TransformCoord(XMLoadFloat4(&FrustumPlane.m_vWorldPoints[i]), ViewMatrixInv));
	}

	XMStoreFloat4(&FrustumPlane.m_vWorldPlane[0], XMPlaneFromPoints(XMLoadFloat4(&FrustumPlane.m_vWorldPoints[1]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[5]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[6])));
	XMStoreFloat4(&FrustumPlane.m_vWorldPlane[1], XMPlaneFromPoints(XMLoadFloat4(&FrustumPlane.m_vWorldPoints[4]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[0]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[3])));
	XMStoreFloat4(&FrustumPlane.m_vWorldPlane[2], XMPlaneFromPoints(XMLoadFloat4(&FrustumPlane.m_vWorldPoints[4]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[5]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[1])));
	XMStoreFloat4(&FrustumPlane.m_vWorldPlane[3], XMPlaneFromPoints(XMLoadFloat4(&FrustumPlane.m_vWorldPoints[3]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[2]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[6])));
	XMStoreFloat4(&FrustumPlane.m_vWorldPlane[4], XMPlaneFromPoints(XMLoadFloat4(&FrustumPlane.m_vWorldPoints[5]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[4]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[7])));
	XMStoreFloat4(&FrustumPlane.m_vWorldPlane[5], XMPlaneFromPoints(XMLoadFloat4(&FrustumPlane.m_vWorldPoints[0]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[1]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[2])));

	m_FrustumPlane[FRUSTUM_STATIC].push_back(FrustumPlane);
}

void CFrustum::Item_Update()
{
	if (m_listFrustumDesc[FRUSTUM_ITEM].empty())
		return;

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);
	_matrix			ViewMatrixInv = XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW));

	for (auto& pair = m_listFrustumDesc[FRUSTUM_ITEM].begin(); pair != m_listFrustumDesc[FRUSTUM_ITEM].end();)
	{
		_vector vItemPosition = pair->first->Get_State(CTransform::STATE_TRANSLATION);
		if (View_Frustum_Culling(vItemPosition, 1.f))
		{
			_vector vItemLook = pair->first->Get_State(CTransform::STATE_LOOK);
			_vector vViewLook = XMLoadFloat4((_float4*)(&ViewMatrixInv.r[2]));

			if (0.f > XMVectorGetX(XMVector3Dot(vItemLook, vViewLook)))
			{
				Safe_Release(pair->first);
				Safe_Release(pair->second);
				pair = m_listFrustumDesc[FRUSTUM_ITEM].erase(pair);
				continue;
			}
		}
		
		_matrix			ViewMatrixInv = XMMatrixInverse(nullptr, pair->first->Get_WorldMatrix());
		_matrix			ProjMatrixInv = XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ));

		FRUSTUMPLANE FrustumPlane;


		for (_uint i = 0; i < 8; ++i)
		{
			XMStoreFloat4(&FrustumPlane.m_vWorldPoints[i], XMVector3TransformCoord(XMLoadFloat4(&m_vOriginalPoints[i]), ProjMatrixInv));
			XMStoreFloat4(&FrustumPlane.m_vWorldPoints[i], XMVector3TransformCoord(XMLoadFloat4(&FrustumPlane.m_vWorldPoints[i]), ViewMatrixInv));
		}

		XMStoreFloat4(&FrustumPlane.m_vWorldPlane[0], XMPlaneFromPoints(XMLoadFloat4(&FrustumPlane.m_vWorldPoints[1]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[5]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[6])));
		XMStoreFloat4(&FrustumPlane.m_vWorldPlane[1], XMPlaneFromPoints(XMLoadFloat4(&FrustumPlane.m_vWorldPoints[4]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[0]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[3])));
		XMStoreFloat4(&FrustumPlane.m_vWorldPlane[2], XMPlaneFromPoints(XMLoadFloat4(&FrustumPlane.m_vWorldPoints[4]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[5]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[1])));
		XMStoreFloat4(&FrustumPlane.m_vWorldPlane[3], XMPlaneFromPoints(XMLoadFloat4(&FrustumPlane.m_vWorldPoints[3]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[2]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[6])));
		XMStoreFloat4(&FrustumPlane.m_vWorldPlane[4], XMPlaneFromPoints(XMLoadFloat4(&FrustumPlane.m_vWorldPoints[5]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[4]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[7])));
		XMStoreFloat4(&FrustumPlane.m_vWorldPlane[5], XMPlaneFromPoints(XMLoadFloat4(&FrustumPlane.m_vWorldPoints[0]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[1]), XMLoadFloat4(&FrustumPlane.m_vWorldPoints[2])));

		m_FrustumPlane[FRUSTUM_ITEM].push_back(FrustumPlane);

		++pair;
		
	}
	RELEASE_INSTANCE(CPipeLine);
}

_bool CFrustum::View_Frustum_Culling(_fvector vWorldPoint, _float fRange)
{
	for (_uint i = 0; i < 6; ++i)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_FrustumPlane[FRUSTUM_STATIC].front().m_vWorldPlane[i]), vWorldPoint)))
		{//default 뷰 절두체
		
			return false;
		}
	
	}

	return true;
}

void CFrustum::Item_Frustum_Culling(_fvector vWorldPoint, _float fRange, CRenderer::RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	if (m_FrustumPlane[FRUSTUM_ITEM].empty())
		return;

	auto& pair = m_listFrustumDesc[FRUSTUM_ITEM].begin();

	for (auto& elem : m_FrustumPlane[FRUSTUM_ITEM])
	{
		_bool bFlag = false;
		for (_uint i = 0; i < 6; ++i)
		{
			if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&elem.m_vWorldPlane[i]), vWorldPoint)))
			{
				++pair;
				bFlag = true;
				break;
			}
		}

		if (!bFlag)
		{
			pair->second->Add_RenderGroup(eRenderGroup, pGameObject);
			++pair;
		}
	}

}

void CFrustum::Free()
{
	for (_int i = 0; i < FRUSTUM_END; ++i)
	{
		for (auto& pair : m_listFrustumDesc[i])
		{
			Safe_Release(pair.first);
			Safe_Release(pair.second);
		}
		m_listFrustumDesc[i].clear();
		m_FrustumPlane[i].clear();
	}

	for (auto& pair : m_vecObjectList)
	{
		Safe_Release(pair.first);
	}
	m_vecObjectList.clear();
}

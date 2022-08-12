#include "stdafx.h"
#include "../Public/Item.h"
#include "GameInstance.h"

CItem::CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CItem::CItem(const CItem& rhs)
    :CGameObject(rhs)
{
}

HRESULT CItem::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CItem::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
}

void CItem::LateTick(_float fTimeDelta)
{
}

HRESULT CItem::Render()
{
    return S_OK;
}



_bool CItem::Picking(CVIBuffer* pVIBufferCom,  _float3& vOut)
{
    RAY tagMouseRay = CMath_Utility::Get_MouseRayInWorldSpace();
    _float3 fDist;

    return CMath_Utility::Picking(pVIBufferCom, m_pTransformCom, tagMouseRay, &vOut);
}

void CItem::Free()
{
    __super::Free();
    
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pTextureCom);
    //�ش� Ŭ������ �ִ� �������� �׻� safe_release���ֱ�
}

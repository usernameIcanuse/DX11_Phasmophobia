#include "stdafx.h"
#include "Ghost_Behavior.h"
#include "GameInstance.h"


CGhost_Behavior::CGhost_Behavior(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CGhost_Behavior::CGhost_Behavior(const CGhost_Behavior& rhs)
	:CGameObject(rhs)
{
}

HRESULT CGhost_Behavior::Initialize_Prototype()
{
	if (__super::Initialize_Prototype())
		return E_FAIL;



	return S_OK;
}

HRESULT CGhost_Behavior::Initialize(void* pArg)
{
	
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pOwnerTransform = (CTransform*)pArg;
		Safe_AddRef(m_pOwnerTransform);
	}

	GAMEINSTANCE->Add_EventObject(CGame_Manager::EVENT_GHOST, this);

	/*생성과 동시에 플레이어 위치를 알고 있음?
	근데 만약에 플레이어 생성 전에 얘가 만들어지면 안됨*/

	return S_OK;
}

void CGhost_Behavior::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
}

void CGhost_Behavior::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


}

HRESULT CGhost_Behavior::Render()
{

	return S_OK;
}

void CGhost_Behavior::OnEventMessage(const _tchar* pMessage)
{
	if (0 == lstrcmp(TEXT("Normal_Operation"), pMessage))
	{
		/*
			귀신 디폴트로 뛰어다님
			이동하다가 네비 메쉬에서 벽쪽에 부딪혔다면
			방향 틀어서 이동

			이동은 이 안에서
			귀신 모델 애니메이션은 CGhost 내부에서
		*/
		
	}
	else if (0 == lstrcmp(TEXT("Event"), pMessage))
	{
		/*이벤트 그냥 소리 효과나 플레이어 주변에 정지한 상태로 바라보기?*/
	}
	else if (0 == lstrcmp(TEXT("Attack"), pMessage))
	{
		/*
			스폰 내부로 이동해서 거기서 부터 플레이어 따라옴
			플레이어 따라오는 길찾기(에이 스타?) 만들어놓기 매번 갱신해야함
		*/
	}
}


CGhost_Behavior* CGhost_Behavior::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGhost_Behavior* pInstance = new CGhost_Behavior(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGhost_Behavior");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGhost_Behavior::Clone(void* pArg)
{
	CGhost_Behavior* pInstance = new CGhost_Behavior(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGhost_Behavior");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Behavior::Free()
{
	__super::Free();

	Safe_Release(m_pOwnerTransform);

}

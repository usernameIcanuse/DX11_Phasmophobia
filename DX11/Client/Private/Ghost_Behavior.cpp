#include "stdafx.h"
#include "Ghost_Behavior.h"
#include "GameInstance.h"
#include "Player.h"


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


	if (FAILED(Setup_Component(pArg)))
		return E_FAIL;
	 
	CPlayer* pPlayer = (CPlayer*)GAMEINSTANCE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
	m_pPlayerInHouse = pPlayer->IsInHouse();
	m_pPlayerTransform = (CTransform*)pPlayer->Get_Component(CGameObject:: m_pTransformTag);
	Safe_AddRef(m_pPlayerTransform);

	GAMEINSTANCE->Add_EventObject(CGame_Manager::EVENT_GHOST, this);
	m_pEventFunc = std::bind(&CGhost_Behavior::Normal_Operation, std::placeholders::_1, std::placeholders::_2);

	return S_OK;
}

HRESULT CGhost_Behavior::Setup_Component(void* pArg )
{
	/* For.Com_Navigation*/
	CNavigation::NAVIDESC	NaviDesc;
	ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));
	NaviDesc.m_iCurrentIndex = 0;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation_House"), TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_iSpawnPointIndex = m_pNavigationCom->Find_PosIndex(*(_vector*)pArg);
	}

#ifdef _DEBUG
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
#endif
	return S_OK;
}

void CGhost_Behavior::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
}

void CGhost_Behavior::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef _DEBUG
	//m_pRendererCom->Add_DebugRenderGroup(m_pNavigationCom);
#endif
}

HRESULT CGhost_Behavior::Render()
{

	return S_OK;
}

void CGhost_Behavior::Call_EventFunc(_float fTimeDelta)
{
	m_pEventFunc(this,fTimeDelta);
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
		m_pEventFunc = std::bind(&CGhost_Behavior::Normal_Operation, std::placeholders::_1, std::placeholders::_2);

	}

	else if (0 == lstrcmp(TEXT("Event"), pMessage))
	{
		/*이벤트 그냥 소리 효과나 플레이어 주변에 정지한 상태로 바라보기?*/
		m_pOwnerTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION));
		m_pEventFunc = std::bind(&CGhost_Behavior::Event, std::placeholders::_1, std::placeholders::_2);
	}

	else if (0 == lstrcmp(TEXT("Attack"), pMessage))
	{
		/*
			스폰 내부로 이동해서 거기서 부터 플레이어 따라옴
			플레이어 따라오는 길찾기(에이 스타?) 만들어놓기 매번 갱신해야함
		*/
		m_fIdleTime = 3.f;
		m_pEventFunc = std::bind(&CGhost_Behavior::Attack, std::placeholders::_1, std::placeholders::_2);
	}
}


void CGhost_Behavior::Normal_Operation(_float fTimeDelta)
{

}

void CGhost_Behavior::Event(_float fTimeDelta)
{

}

void CGhost_Behavior::Attack(_float fTimeDelta)
{
	if (false == (*m_pPlayerInHouse))
		return;

	m_fIdleTime -= fTimeDelta;
	if (0.f > m_fIdleTime)
	{
		m_pOwnerTransform->Go_Straight(fTimeDelta * 1.3f/*, m_pNavigationCom*/);
	}

	m_pOwnerTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION));
}

void CGhost_Behavior::Move(_float fTimeDelta)
{
	m_pOwnerTransform->Go_Straight(fTimeDelta*4.f, m_pNavigationCom);
}

void CGhost_Behavior::Set_SpawnPointindex()
{
	m_pNavigationCom->Set_CurrentIndex(m_iSpawnPointIndex);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 60);

	_float fDegree = dis(gen) - 30;

	m_pOwnerTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fDegree));
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
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pPlayerTransform);
#ifdef _DEBUG
	Safe_Release(m_pRendererCom);
#endif

}

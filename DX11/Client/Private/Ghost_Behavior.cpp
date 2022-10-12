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

	if (FAILED(Setup_Component()))
		return E_FAIL;

	m_pPlayerTransform = (CTransform*)GAMEINSTANCE->Get_Component(LEVEL_STAGE1, TEXT("Layer_Player"), CGameObject::m_pTransformTag);
	Safe_AddRef(m_pPlayerTransform);

	GAMEINSTANCE->Add_EventObject(CGame_Manager::EVENT_GHOST, this);

	m_fChangeDir = 5.f;
	m_fRadian = XMConvertToRadians(10.f);
	/*생성과 동시에 플레이어 위치를 알고 있음?
	근데 만약에 플레이어 생성 전에 얘가 만들어지면 안됨 순서를 맞춰줘야함?*/

	return S_OK;
}

HRESULT CGhost_Behavior::Setup_Component()
{
	/* For.Com_Navigation*/
	CNavigation::NAVIDESC	NaviDesc;
	ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));
	NaviDesc.m_iCurrentIndex = 0;

	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Navigation_Ghost"), TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
#endif
	return S_OK;
}

void CGhost_Behavior::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fChangeDir -= fTimeDelta;
	
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
		//m_pOwnerTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION));
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
	_float fRadian = m_fRadian * 0.05f;
	m_fRadian -= fRadian;
	if (0.f > m_fRadian)
		m_fRadian = 0.f;

	m_pOwnerTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
	if (FAILED(m_pOwnerTransform->Go_Straight(fTimeDelta, m_pNavigationCom)))
	{
		m_fRadian = XMConvertToRadians(35.f);
		fRadian = m_fRadian * 0.1f;
		m_fRadian -= fRadian;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, 180);

		m_fChangeDir = (_float)(dis(gen) % 6 + 1);
	}
	
	if (0.f > m_fChangeDir)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, 360);

		_int iValue = dis(gen) % 6 + 1;
		m_fChangeDir = (_float)abs(iValue);
 		m_fRadian = XMConvertToRadians(dis(gen));
		m_fRadian -= XMConvertToRadians(180.f);
	}
}

void CGhost_Behavior::Event(_float fTimeDelta)
{

}

void CGhost_Behavior::Attack(_float fTimeDelta)
{
	//m_pOwnerTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION));

	m_fIdleTime -= fTimeDelta;
	if (0.f > m_fIdleTime)
	{
		m_pOwnerTransform->Go_Straight(fTimeDelta * 1.3f, m_pNavigationCom);
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
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pPlayerTransform);
#ifdef _DEBUG
	Safe_Release(m_pRendererCom);
#endif

}

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

	/*������ ���ÿ� �÷��̾� ��ġ�� �˰� ����?
	�ٵ� ���࿡ �÷��̾� ���� ���� �갡 ��������� �ȵ�*/

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
			�ͽ� ����Ʈ�� �پ�ٴ�
			�̵��ϴٰ� �׺� �޽����� ���ʿ� �ε����ٸ�
			���� Ʋ� �̵�

			�̵��� �� �ȿ���
			�ͽ� �� �ִϸ��̼��� CGhost ���ο���
		*/
		
	}
	else if (0 == lstrcmp(TEXT("Event"), pMessage))
	{
		/*�̺�Ʈ �׳� �Ҹ� ȿ���� �÷��̾� �ֺ��� ������ ���·� �ٶ󺸱�?*/
	}
	else if (0 == lstrcmp(TEXT("Attack"), pMessage))
	{
		/*
			���� ���η� �̵��ؼ� �ű⼭ ���� �÷��̾� �����
			�÷��̾� ������� ��ã��(���� ��Ÿ?) �������� �Ź� �����ؾ���
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

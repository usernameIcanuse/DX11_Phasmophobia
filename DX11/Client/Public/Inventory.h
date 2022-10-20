#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
class CVIBuffer_Rect;
class CShader;
class CRenderer;
END


BEGIN(Client)

class Player;
class CItem;

class CInventory final : public CGameObject
{
private:
	CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInventory(const CInventory& rhs);
	virtual ~CInventory() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);

private:
	void	Add_Item(CItem* pItem);
	void	Drop_Item();//아이템 버리기
	void	Change_Item();

	void	Install_Item(_float3 _vInstallPos,COLLISION_TYPE _eCollisionType, _float4 vLook, CItem* pConnectObject = nullptr);//아이템 설치
	void	Turn_Switch();

	void	Item_TempModel(_float3 _vInstallPos, COLLISION_TYPE _eCollisionType, _float4 vLook, CItem* pConnectObject = nullptr);
	void	Frequency_Control(_long _lMouseMove);

private:
	HRESULT	Setup_Component();


private:
	CCollider* m_pRayCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	////직교투영////////////////////////
	_float4x4  m_ViewMatrix, m_ProjMatrix;
	_float m_fSizeX, m_fSizeY, m_fX, m_fY;
	_bool		m_bOnMouse = true;
	////////////////////////////////////

	vector					<CItem*>	m_vInventory;
	_uint					m_iIndex = 0;
	CItem*					m_pSpotLight = nullptr;

	class CPlayer*			m_pPlayer = nullptr;
	CTransform*				m_pPlayerTransform = nullptr;

	_float			m_fDist = FLT_MAX;
	CGameObject* m_pItem = nullptr;
	class CDoor* m_pDoor = nullptr;

	_float3			m_vColliderPos;
	COLLISION_TYPE	m_eColliderType = COLLISION_TYPE::TYPE_END;
	_float4			m_vColliderLook;

	_bool			m_bGrab = false;
	CGameObject* m_pTripod = nullptr;

	_bool			m_bLockCursor = true;




	
public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

public:
	static CInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;//clone을 할 때 플레이어의 주소를 갖다줌
	virtual void Free() override;

};

END

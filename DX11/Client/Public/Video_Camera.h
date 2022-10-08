#pragma once
#include "Item.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCamera_Renderer;
class CTexture;
END

BEGIN(Client)

class CTripod;
class CCamera_Screen;
class CVideo_Camera final: public CItem
{
private:
	CVideo_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVideo_Camera(const CVideo_Camera& rhs);
	virtual ~CVideo_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	//virtual void OnEventMessage(const _tchar* pMessage);

public:

	virtual _bool Install(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem = nullptr);
	virtual void Set_TempModel_Pos(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem = nullptr);

	virtual void MalFunction(_float fTimeDelta = 0.f) {}
	virtual void Normal_Operation(_float fTimeDelta = 0.f) {}

	virtual void Turn_Switch();

	virtual void Set_Enable(_bool _bEnable);
	virtual void Adjust_Item(CTransform* _pPlayerTransform);
public:
	void	Connect_Tripod(CTripod* pTripod);
	void	Disconnect_Tripod();

private:
	CTripod* m_pTripod = nullptr;
	CItem* m_pTempCameraModel = nullptr;
	CCamera_Screen* m_pCameraScreen = nullptr;
	CTexture* m_pNormalTex = nullptr;

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

private:
	virtual	HRESULT	Setup_Component() override;
	HRESULT			Setup_TempModel();
	HRESULT			Setup_Screen();
public:
	static CVideo_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
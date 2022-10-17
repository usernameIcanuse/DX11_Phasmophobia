#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;

END

BEGIN(Client)

class CComputer final : public CGameObject
{
public:
	typedef struct tagComputerDesc
	{
		_float4x4 TruckMat;
		_float4x4 WorldMat;
		_int	iNumCamera;
	}COMPUTERDESC;


private:
	CComputer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComputer(const CComputer& rhs);
	virtual ~CComputer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);

public:
	HRESULT Set_Screen_RSV(CShader* _pShaderCom);

private:
	CShader* m_pShaderCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CCollider* m_pOBBCom = nullptr;


	class CKeyBoard* m_pKeyBoard = nullptr;
	class CMouse* m_pMouse = nullptr;
	
private:
	list<class CVideo_Camera*> m_listCameraOff;//꺼진 카메라
	list<class CVideo_Camera*> m_listCameraOn; //켜진 카메라

	_int		m_iCurrCamIndex = 0;//켜진 카메라중 보고 싶은 화면 가리킬 인덱스
	

private:
	virtual	HRESULT	Setup_Component();
	HRESULT			Connect_Camera(_int _iNumCamera, _float4x4 _TruckMat);
	HRESULT			Setup_Screen();
	HRESULT			Setup_Computer(_float4x4 _TruckMat);

public:
	static CComputer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
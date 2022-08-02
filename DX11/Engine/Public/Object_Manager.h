#pragma once

/* ��ü���� ������ �����ϳ�. */
/* ������ü�� ã�Ƽ� �����Ͽ� ��(����)���� ��Ƽ� �����ϴ�.*/

#include "Base.h"

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);

public:
	HRESULT Reserve_Container(_uint iNumLevels);

public:
	/* ������ü�� �߰��Ѵ�. */
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);

	/* ������ü�����Ӥ� �����Ͽ� �纻��ü�� �߰��Ѵ�. */
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, CGameObject** pOut = nullptr , void* pArg = nullptr);

	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	void Clear(_uint iLevelIndex);


private:
	map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef map<const _tchar*, class CGameObject*>	PROTOTYPES;

private:
	map<const _tchar*, class CLayer*>*				m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>		LAYERS;

	_uint					m_iNumLevels = 0;


private:
	class CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);




public:
	virtual void Free() override;
};

END
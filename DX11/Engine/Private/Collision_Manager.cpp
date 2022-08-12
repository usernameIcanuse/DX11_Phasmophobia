#include "Collision_Manager.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Collider_Mesh.h"

IMPLEMENT_SINGLETON(CCollision_Manager)

CCollision_Manager::CCollision_Manager()
	:m_arrCheck{}
{
}

void CCollision_Manager::Initialize()
{
	CheckGroup(COLLISION_TYPE::PLAYER_ATTACK, COLLISION_TYPE::MONSTER);
	CheckGroup(COLLISION_TYPE::MONSTER_ATTACK, COLLISION_TYPE::PLAYER);
	CheckGroup(COLLISION_TYPE::PLAYER, COLLISION_TYPE::OBJECT);
	CheckGroup(COLLISION_TYPE::PLAYER, COLLISION_TYPE::MONSTER);
	CheckGroup(COLLISION_TYPE::PLAYER, COLLISION_TYPE::ITEM);
	//CheckGroup(COLLISION_TYPE::PLAYER, COLLISION_TYPE::PLAYER);

	//CheckGroup(COLLISION_TYPE::PLAYER_ATTACK, COLLISION_TYPE::PLAYER_ATTACK);
	//CheckGroup(COLLISION_TYPE::PLAYER, COLLISION_TYPE::MONSTER);
	//CheckGroup(COLLISION_TYPE::MONSTER, COLLISION_TYPE::MONSTER);
}

void CCollision_Manager::Tick()
{
	for (UINT iRow = 0; iRow < (UINT)COLLISION_TYPE::TYPE_END; ++iRow)
	{
		for (UINT iCol = iRow; iCol < (UINT)COLLISION_TYPE::TYPE_END; ++iCol) // üũ �ڽ� �ݸ� �ؾߵǱ� ������ �÷��� row����
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				//auto Handle = async(&CCollision_Manager::CollisionGroupUpdate, this, (COLLISION_TYPE)iRow, (COLLISION_TYPE)iCol);

				CollisionGroupUpdate((COLLISION_TYPE)iRow, (COLLISION_TYPE)iCol);
			}
		}
	}

	Clear_ColliderList();
}

void CCollision_Manager::CheckGroup(COLLISION_TYPE _eLeft, COLLISION_TYPE _eRight)
{
	UINT iRow = (UINT)_eLeft; // ��
	UINT iCol = (UINT)_eRight; // ��

	if (iRow > iCol)
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol)) // �̹� �� �ڸ��� 1�̸�
		m_arrCheck[iRow] &= ~(1 << iCol); // ���ְ�
	else
		m_arrCheck[iRow] |= (1 << iCol); // 1�� �÷� ��ŭ �о �־���..?
}

void CCollision_Manager::Reset()
{
	memset(m_arrCheck, 0, sizeof(UINT) * (UINT)COLLISION_TYPE::TYPE_END);

	for (UINT i = 0; i < (UINT)COLLISION_TYPE::TYPE_END; ++i)
		m_ColliderList->clear();
}

void CCollision_Manager::Add_Collider(CCollider* pCollider)
{
	m_ColliderList[(UINT)pCollider->Get_Collision_Type()].push_back(pCollider);

	pCollider->Set_WeakPtr(&m_ColliderList[(UINT)pCollider->Get_Collision_Type()].back());
}

void CCollision_Manager::Erase_Collider(CCollider* pCollider)
{
	m_Erase_ColliderIDList.push_back(pCollider->Get_ID());
}

list<CCollider*>* CCollision_Manager::Get_ColliderList(COLLISION_TYPE _eType)
{
	return &m_ColliderList[(_uint)_eType];
}

void CCollision_Manager::Clear_ColliderList()
{
	for (auto& ColliderList_Type : m_ColliderList)
	{
		for (auto& elem_Collider : ColliderList_Type)
		{
			RETURN_WEAKPTR(elem_Collider);
		}

		ColliderList_Type.clear();
	}

	for (_ulong& elem_ID : m_Erase_ColliderIDList)
	{
		m_mapColInfo.erase(elem_ID / 100000);
		m_mapColInfo.erase(elem_ID % 100000);
	}
	m_Erase_ColliderIDList.clear();

	
}

void CCollision_Manager::CollisionGroupUpdate(COLLISION_TYPE _eLeft, COLLISION_TYPE _eRight)
{

	map<ULONGLONG, bool>::iterator iter;

	for (auto LeftIter = m_ColliderList[(UINT)_eLeft].begin(); LeftIter != m_ColliderList[(UINT)_eLeft].end(); )
	{
		if (!(*LeftIter))
		{
			LeftIter = m_ColliderList[(UINT)_eLeft].erase(LeftIter);
			continue;
		} //�̹� ������ ��ü�� ����Ʈ���� ����� �ѱ��.

		for (auto RightIter = m_ColliderList[(UINT)_eRight].begin(); RightIter != m_ColliderList[(UINT)_eRight].end(); )
		{
			if (!(*RightIter))
			{
				RightIter = m_ColliderList[(UINT)_eRight].erase(RightIter);
				continue;
			} //�̹� ������ ��ü�� ����Ʈ���� ����� �ѱ��.

			if (*LeftIter == *RightIter) // �� �ڽŰ� �浹 ����
			{
				++RightIter;
				continue;
			}

			_float fDistance = 0.f;

			//��¥ �浹 �˻�
			if (Is3DCollision((*LeftIter), (*RightIter), &fDistance))
			{
				COLLIDER_ID ID;
				ID.ID = 0;
				ID.Left_id = (*LeftIter)->Get_ID();		// 4����Ʈ
				ID.Right_id = (*RightIter)->Get_ID();		// 4����Ʈ
				//���ļ� ���� 8����Ʈ�� ���� �Ȱ�ħ.
				/*string id_to_string = to_string(ID.Left_id) + to_string(ID.Right_id);

				ID.ID = stoll(id_to_string.c_str());*/

				_ulonglong Byte8ID;

				Byte8ID = (_ulonglong)ID.Left_id * 100000;
				Byte8ID += ID.Right_id;

				iter = m_mapColInfo.find(Byte8ID);



				//�浹 ������ �ƿ� �̵�� ������ ���
				if (m_mapColInfo.end() == iter)
				{
					m_mapColInfo.insert(make_pair(Byte8ID, false)); // �ƿ� ������� �ȵ��ִ� ��Ȳ�̹Ƿ� �ʿ� �߰�
					iter = m_mapColInfo.find(Byte8ID); // ���� �� �ٽ� ���ͷ� �ް�
				}

				//���� �浹 ��
				if (iter->second) // ���ݵ� �浹���ε� �������� �浹���� ��Ȳ
				{
					if (!(*LeftIter) || !(*RightIter)) // ���� �ϳ��� ��ȿ���� ���� ���
					{
						(*LeftIter)->Get_Owner()->On_Collision_Exit((*RightIter));
						(*RightIter)->Get_Owner()->On_Collision_Exit((*LeftIter));
						iter->second = false;
						//m_mapColInfo.erase(iter);
					}
					else
					{
						(*LeftIter)->Get_Owner()->On_Collision_Stay((*RightIter));
						(*RightIter)->Get_Owner()->On_Collision_Stay((*LeftIter));

					}


				}
				else // �̹��� ���� �浹
				{
					if ((*LeftIter) && (*RightIter)) // �Ѵ� Ȱ��ȭ�� ���� ����
					{
						(*LeftIter)->Get_Owner()->On_Collision_Enter((*RightIter));
						(*RightIter)->Get_Owner()->On_Collision_Enter((*LeftIter));
						iter->second = true;

					}
					else
					{
						//m_mapColInfo.erase(iter);
					}
				}
			}
			else // ���� �浹������ ����
			{
				if (fDistance > 100.f)
				{
					++RightIter;
					continue;
				}
					

				COLLIDER_ID ID;
				ID.ID = 0;
				ID.Left_id = (*LeftIter)->Get_ID();		// 4����Ʈ
				ID.Right_id = (*RightIter)->Get_ID();		// 4����Ʈ
				//���ļ� ���� 8����Ʈ�� ���� �Ȱ�ħ.
				/*string id_to_string = to_string(ID.Left_id) + to_string(ID.Right_id);

				ID.ID = stoll(id_to_string.c_str());*/

				_ulonglong Byte8ID;

				Byte8ID = (_ulonglong)ID.Left_id * 100000;
				Byte8ID += ID.Right_id;

				iter = m_mapColInfo.find(Byte8ID);

				if (iter == m_mapColInfo.end())
				{
					++RightIter;
					continue;
				}


				if (iter->second) // ��ݱ��� �浹���̾���, �浹�� �� ����� ����
				{
					(*LeftIter)->Get_Owner()->On_Collision_Exit((*RightIter));
					(*RightIter)->Get_Owner()->On_Collision_Exit((*LeftIter));
					iter->second = false;
					//m_mapColInfo.erase(iter);
				}
			}

			++RightIter;
		}

		++LeftIter;

	}
}

bool CCollision_Manager::Is3DCollision(CCollider* _pLeft, CCollider* _pRight, _float* _fDistance)
{

	//if (!IsSphereCollision(_pLeft->Get_Pre_Collider(), _pRight->Get_Pre_Collider()))
	//{
	//	return false;
	//}


	if ((_pLeft->Get_Collider_Shape() == COLLIDER_SHAPE::OBB)
		&& (_pRight->Get_Collider_Shape() == COLLIDER_SHAPE::OBB))
	{
		return IsOBBCollision(_pLeft, _pRight);
	}// �簢 - �簢
	else if((_pLeft->Get_Collider_Shape() == COLLIDER_SHAPE::SPHERE)
		&& (_pRight->Get_Collider_Shape() == COLLIDER_SHAPE::SPHERE))
	{
		return IsSphereCollision(_pLeft, _pRight);
	}// �簢 - ��, �� - ��

	else if ((_pLeft->Get_Collider_Shape() == COLLIDER_SHAPE::SPHERE)
		&& (_pRight->Get_Collider_Shape() == COLLIDER_SHAPE::MESH))
	{
		return IsMesh_To_SphereCollision(_pLeft, _pRight);
	}
	else if ((_pLeft->Get_Collider_Shape() == COLLIDER_SHAPE::MESH)
		&& (_pRight->Get_Collider_Shape() == COLLIDER_SHAPE::SPHERE))
	{
		return IsMesh_To_SphereCollision(_pRight, _pLeft);
	}
	else if ((_pLeft->Get_Collider_Shape() == COLLIDER_SHAPE::RAY)
		&& (_pRight->Get_Collider_Shape() == COLLIDER_SHAPE::SPHERE))
	{
		return IsRay_To_SphereCollision(_pLeft, _pRight);
	}
	else if ((_pLeft->Get_Collider_Shape() == COLLIDER_SHAPE::SPHERE)
		&& (_pRight->Get_Collider_Shape() == COLLIDER_SHAPE::RAY))
	{
		return IsRay_To_SphereCollision(_pRight, _pLeft);
	}

	return IsSphereCollision(_pLeft, _pRight, _fDistance);
}

bool CCollision_Manager::IsOBBCollision(CCollider* _pLeft, CCollider* _pRight, _float* _fDistance)
{
	OBBINFO	LeftBox = static_cast<CCollider_OBB*>(_pLeft)->Get_OBBInfo();
	OBBINFO	RightBox = static_cast<CCollider_OBB*>(_pRight)->Get_OBBInfo();


	D3DXVECTOR3 Axis_c1[3] = {
		LeftBox.tVertex[1] - LeftBox.tVertex[0],
		LeftBox.tVertex[3] - LeftBox.tVertex[0],
		LeftBox.tVertex[4] - LeftBox.tVertex[0] };

	D3DXVECTOR3 Axis_c2[3] = {
		RightBox.tVertex[1] - RightBox.tVertex[0],
		RightBox.tVertex[3] - RightBox.tVertex[0],
		RightBox.tVertex[4] - RightBox.tVertex[0] };

	// 2. ���� �������� �˻� 
	for (int i = 0; i < 3; ++i)
	{
		// ������ �Ǵ� �� ����
		D3DXVECTOR3 Axis_norm;
		D3DXVec3Normalize(&Axis_norm, &Axis_c1[i]);

		// 2 - 1. �࿡���� ���� A�� B ����
		map<float, UINT> mValue_Right;
		map<float, UINT> mValue_Left;
		UINT minIndex_Right, maxIndex_Right;
		UINT minIndex_Left, maxIndex_Left;

		//���� ª�������� ������ ã�Ƴ���.
		for (int v = 0; v < sizeof(RightBox.tVertex) / sizeof(D3DXVECTOR3); ++v)
		{
			mValue_Right.insert(make_pair(fabsf(D3DXVec3Dot(&Axis_norm, &RightBox.tVertex[v])), v));
			mValue_Left.insert(make_pair(fabsf(D3DXVec3Dot(&Axis_norm, &LeftBox.tVertex[v])), v));
		}


		// �񱳵� ��� ����� �ִ�, ���� ������ Index ��ȣ

		map<float, UINT>::iterator Right_iter;
		Right_iter = mValue_Right.end();
		--Right_iter;

		minIndex_Right = mValue_Right.begin()->second;
		maxIndex_Right = Right_iter->second;

		map<float, UINT>::iterator Left_iter;
		Left_iter = mValue_Left.end();
		--Left_iter;

		minIndex_Left = mValue_Left.begin()->second;
		maxIndex_Left = Left_iter->second;


		// 1, 3, 4 ������ ���� ŭ
		FLOAT A, B, Dist;
		D3DXVECTOR3 vec;

		FLOAT A1 = (D3DXVec3Dot(&Axis_norm, &LeftBox.tVertex[maxIndex_Left]));
		FLOAT A2 = (D3DXVec3Dot(&Axis_norm, &LeftBox.tVertex[minIndex_Left]));

		A = fabs(A1 - A2);


		FLOAT B1 = (D3DXVec3Dot(&Axis_norm, &RightBox.tVertex[maxIndex_Right]));
		FLOAT B2 = (D3DXVec3Dot(&Axis_norm, &RightBox.tVertex[minIndex_Right]));

		B = fabs(B1 - B2);

		A = A * 0.5F;
		B = B * 0.5F;

		// 2 - 2. �� ���������� ���� ������������ ���� Dist ����

		B1 = (D3DXVec3Dot(&Axis_norm, &LeftBox.ObbCenterPos));
		B2 = (D3DXVec3Dot(&Axis_norm, &RightBox.ObbCenterPos));
		Dist = fabsf(B1 - B2);

		// 2 - 3. Dist�� A * 0.5 + B * 0.5 �� ��
		if (Dist > A + B)
			return false; //�浹 ����
		// Dist < A + B �浹 ��
	}

	// 2. ���� �������� �˻� 
	for (int i = 0; i < 3; ++i)
	{
		D3DXVECTOR3 Axis_norm;
		D3DXVec3Normalize(&Axis_norm, &Axis_c2[i]);

		// 2 - 1. �࿡���� ���� A�� B ����
		map<float, UINT> mValue_Right;
		map<float, UINT> mValue_Left;
		UINT minIndex_Right, maxIndex_Right;
		UINT minIndex_Left, maxIndex_Left;

		//���� ª�������� ������ ã�Ƴ���.
		for (int v = 0; v < sizeof(RightBox.tVertex) / sizeof(D3DXVECTOR3); ++v)
		{
			mValue_Right.insert(make_pair(fabsf(D3DXVec3Dot(&Axis_norm, &RightBox.tVertex[v])), v));
			mValue_Left.insert(make_pair(fabsf(D3DXVec3Dot(&Axis_norm, &LeftBox.tVertex[v])), v));
		}


		// �񱳵� ��� ����� �ִ�, ���� ������ Index ��ȣ

		map<float, UINT>::iterator Right_iter;
		Right_iter = mValue_Right.end();
		--Right_iter;

		minIndex_Right = mValue_Right.begin()->second;
		maxIndex_Right = Right_iter->second;

		map<float, UINT>::iterator Left_iter;
		Left_iter = mValue_Left.end();
		--Left_iter;

		minIndex_Left = mValue_Left.begin()->second;
		maxIndex_Left = Left_iter->second;


		// 1, 3, 4 ������ ���� ŭ
		FLOAT A, B, Dist;
		D3DXVECTOR3 vec;

		FLOAT A1 = (D3DXVec3Dot(&Axis_norm, &LeftBox.tVertex[maxIndex_Left]));
		FLOAT A2 = (D3DXVec3Dot(&Axis_norm, &LeftBox.tVertex[minIndex_Left]));

		A = fabs(A1 - A2);


		FLOAT B1 = (D3DXVec3Dot(&Axis_norm, &RightBox.tVertex[maxIndex_Right]));
		FLOAT B2 = (D3DXVec3Dot(&Axis_norm, &RightBox.tVertex[minIndex_Right]));

		B = fabs(B1 - B2);

		A = A * 0.5F;
		B = B * 0.5F;

		// 2 - 2. �� ���������� ���� ������������ ���� Dist ����

		B1 = (D3DXVec3Dot(&Axis_norm, &LeftBox.ObbCenterPos));
		B2 = (D3DXVec3Dot(&Axis_norm, &RightBox.ObbCenterPos));
		Dist = fabsf(B1 - B2);

		// 2 - 3. Dist�� A * 0.5 + B * 0.5 �� ��
		if (Dist > A + B)
			return false; //�浹 ����
	}

	return true;
}

bool CCollision_Manager::IsSphereCollision(CCollider* _pLeft, CCollider* _pRight, _float* _fDistance)
{
	if (!_pLeft || !_pRight)
	{
		return false;
	}

	D3DXVECTOR3 CenterDiff = _pRight->Get_Collider_Position() - _pLeft->Get_Collider_Position();
	float Dist = D3DXVec3Length(&CenterDiff);
	if (_fDistance)
		*_fDistance = Dist;
	Dist = fabsf(Dist);

	float LeftRadius = _pLeft->Get_Collider_Size().x;
	float RightRadius = _pRight->Get_Collider_Size().x;

	if (Dist > LeftRadius + RightRadius)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CCollision_Manager::IsMesh_To_SphereCollision(CCollider* _pLeft, CCollider* _pRight)
{
	_float3		vSphereCenter = _pLeft->Get_Collider_Position(); 
	_float3     vMeshCenter = _pRight->Get_Collider_Position();
	LPD3DXBASEMESH pMesh = static_cast<CCollider_Mesh*>(_pRight)->Get_Collider_Mesh();
	CTransform* pMeshTransform = _pRight->Get_Owner()->Get_Component<CTransform>();


	RAY	 CollisionRay;
	CollisionRay.Dir = vMeshCenter - vSphereCenter;
	CollisionRay.fLength = D3DXVec3Length(&CollisionRay.Dir);
	D3DXVec3Normalize(&CollisionRay.Dir, &CollisionRay.Dir);
	CollisionRay.Pos = vSphereCenter;

	_float3		vPickingMeshOut;

	return CMath_Utillity::Picking_Mesh(pMesh, pMeshTransform, CollisionRay, &vPickingMeshOut);
}

bool CCollision_Manager::IsRay_To_SphereCollision(CCollider* _pLeft, CCollider* _pRight)
{
	RAY	rayCollider = static_cast<CCollider_Ray*>(_pLeft)->Get_ColliderRay();
	D3DXVec3Normalize(&rayCollider.Dir, &rayCollider.Dir);
	
	_float Range = _pRight->Get_Collider_Size().x;
	_float3 fSpherePos = _pRight->Get_Collider_Position();

	_float3 SphereRay = fSpherePos - rayCollider.Pos;

	_float Dotproduct = D3DXVec3Dot(&rayCollider.Dir, &SphereRay);

	if (Dotproduct > 0.f && rayCollider.fLength > Dotproduct)
	{
		_float3 Projection = Dotproduct * rayCollider.Dir;

		Projection += rayCollider.Pos;

		_float3 Length = fSpherePos - Projection;

		_float fDist = D3DXVec3Length(&Length);


		if (Range > fDist)
		{
			return true;
		}
	}

	return false;
}

void CCollision_Manager::Free()
{
	__super::Free();

	Clear_ColliderList();

	delete this;
}

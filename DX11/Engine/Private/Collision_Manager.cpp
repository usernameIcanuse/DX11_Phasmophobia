#include "Collision_Manager.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Collider.h"

IMPLEMENT_SINGLETON(CCollision_Manager)

CCollision_Manager::CCollision_Manager()
	:m_arrCheck{}
{
}

void CCollision_Manager::Initialize()
{
	
	CheckGroup(COLLISION_TYPE::SIGHT, COLLISION_TYPE::ITEM);
	CheckGroup(COLLISION_TYPE::SIGHT, COLLISION_TYPE::CAMERA);
	CheckGroup(COLLISION_TYPE::SIGHT, COLLISION_TYPE::TRIPOD);
	CheckGroup(COLLISION_TYPE::SIGHT, COLLISION_TYPE::THERMOMETER);
	CheckGroup(COLLISION_TYPE::SIGHT, COLLISION_TYPE::OBJECT);
	CheckGroup(COLLISION_TYPE::SIGHT, COLLISION_TYPE::WALL);

	CheckGroup(COLLISION_TYPE::THERMOMETER, COLLISION_TYPE::ATMOSPHERE);
	CheckGroup(COLLISION_TYPE::THERMOMETER, COLLISION_TYPE::GHOST_AREA);
	CheckGroup(COLLISION_TYPE::THERMOMETER, COLLISION_TYPE::GHOST_SPAWNPOINT);

	CheckGroup(COLLISION_TYPE::ITEM, COLLISION_TYPE::GHOST_AREA);

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
	m_ColliderList[(UINT)pCollider->Get_Type()].push_back(pCollider);
	Safe_AddRef(m_ColliderList[(UINT)pCollider->Get_Type()].back());

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
			Safe_Release(elem_Collider);
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
	return _pLeft->Collision(_pRight);
	//if (!IsSphereCollision(_pLeft->Get_Pre_Collider(), _pRight->Get_Pre_Collider()))
	//{
	//	return false;
	//}


	//if ((_pLeft->Get_Collider_Shape() == COLLIDER_SHAPE::OBB)
	//	&& (_pRight->Get_Collider_Shape() == COLLIDER_SHAPE::OBB))
	//{
	//	return IsOBBCollision(_pLeft, _pRight);
	//}// �簢 - �簢
	//else if((_pLeft->Get_Collider_Shape() == COLLIDER_SHAPE::SPHERE)
	//	&& (_pRight->Get_Collider_Shape() == COLLIDER_SHAPE::SPHERE))
	//{
	//	return IsSphereCollision(_pLeft, _pRight);
	//}// �簢 - ��, �� - ��

	//else if ((_pLeft->Get_Collider_Shape() == COLLIDER_SHAPE::SPHERE)
	//	&& (_pRight->Get_Collider_Shape() == COLLIDER_SHAPE::MESH))
	//{
	//	return IsMesh_To_SphereCollision(_pLeft, _pRight);
	//}
	//else if ((_pLeft->Get_Collider_Shape() == COLLIDER_SHAPE::MESH)
	//	&& (_pRight->Get_Collider_Shape() == COLLIDER_SHAPE::SPHERE))
	//{
	//	return IsMesh_To_SphereCollision(_pRight, _pLeft);
	//}
	//else if ((_pLeft->Get_Collider_Shape() == COLLIDER_SHAPE::RAY)
	//	&& (_pRight->Get_Collider_Shape() == COLLIDER_SHAPE::SPHERE))
	//{
	//	return IsRay_To_SphereCollision(_pLeft, _pRight);
	//}
	//else if ((_pLeft->Get_Collider_Shape() == COLLIDER_SHAPE::SPHERE)
	//	&& (_pRight->Get_Collider_Shape() == COLLIDER_SHAPE::RAY))
	//{
	//	return IsRay_To_SphereCollision(_pRight, _pLeft);
	//}

}



void CCollision_Manager::Free()
{
	//__super::Free();

	Clear_ColliderList();

}

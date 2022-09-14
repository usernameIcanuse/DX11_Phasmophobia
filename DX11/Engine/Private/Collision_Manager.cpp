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
		for (UINT iCol = iRow; iCol < (UINT)COLLISION_TYPE::TYPE_END; ++iCol) // 체크 박스 반만 해야되기 때문에 컬럼은 row부터
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
	UINT iRow = (UINT)_eLeft; // 행
	UINT iCol = (UINT)_eRight; // 열

	if (iRow > iCol)
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol)) // 이미 그 자리가 1이면
		m_arrCheck[iRow] &= ~(1 << iCol); // 빼주고
	else
		m_arrCheck[iRow] |= (1 << iCol); // 1을 컬럼 만큼 밀어서 넣어줌..?
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
		} //이미 삭제된 객체면 리스트에서 지우고 넘긴다.

		for (auto RightIter = m_ColliderList[(UINT)_eRight].begin(); RightIter != m_ColliderList[(UINT)_eRight].end(); )
		{
			if (!(*RightIter))
			{
				RightIter = m_ColliderList[(UINT)_eRight].erase(RightIter);
				continue;
			} //이미 삭제된 객체면 리스트에서 지우고 넘긴다.

			if (*LeftIter == *RightIter) // 나 자신과 충돌 방지
			{
				++RightIter;
				continue;
			}

			_float fDistance = 0.f;

			//진짜 충돌 검사
			if (Is3DCollision((*LeftIter), (*RightIter), &fDistance))
			{
				COLLIDER_ID ID;
				ID.ID = 0;
				ID.Left_id = (*LeftIter)->Get_ID();		// 4바이트
				ID.Right_id = (*RightIter)->Get_ID();		// 4바이트
				//합쳐서 나온 8바이트는 절대 안겹침.
				/*string id_to_string = to_string(ID.Left_id) + to_string(ID.Right_id);

				ID.ID = stoll(id_to_string.c_str());*/

				_ulonglong Byte8ID;

				Byte8ID = (_ulonglong)ID.Left_id * 100000;
				Byte8ID += ID.Right_id;

				iter = m_mapColInfo.find(Byte8ID);



				//충돌 정보가 아예 미등록 상태인 경우
				if (m_mapColInfo.end() == iter)
				{
					m_mapColInfo.insert(make_pair(Byte8ID, false)); // 아예 등록조차 안되있던 상황이므로 맵에 추가
					iter = m_mapColInfo.find(Byte8ID); // 넣은 값 다시 이터로 받고
				}

				//현재 충돌 중
				if (iter->second) // 지금도 충돌중인데 이전에도 충돌중인 상황
				{
					if (!(*LeftIter) || !(*RightIter)) // 둘중 하나가 유효하지 않을 경우
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
				else // 이번에 최초 충돌
				{
					if ((*LeftIter) && (*RightIter)) // 둘다 활성화일 때만 실행
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
			else // 현재 충돌중이지 않음
			{
				if (fDistance > 100.f)
				{
					++RightIter;
					continue;
				}
					

				COLLIDER_ID ID;
				ID.ID = 0;
				ID.Left_id = (*LeftIter)->Get_ID();		// 4바이트
				ID.Right_id = (*RightIter)->Get_ID();		// 4바이트
				//합쳐서 나온 8바이트는 절대 안겹침.
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


				if (iter->second) // 방금까지 충돌중이었음, 충돌이 막 벗어나진 시점
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
	//}// 사각 - 사각
	//else if((_pLeft->Get_Collider_Shape() == COLLIDER_SHAPE::SPHERE)
	//	&& (_pRight->Get_Collider_Shape() == COLLIDER_SHAPE::SPHERE))
	//{
	//	return IsSphereCollision(_pLeft, _pRight);
	//}// 사각 - 원, 원 - 원

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

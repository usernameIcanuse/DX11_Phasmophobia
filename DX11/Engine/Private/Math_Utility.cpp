#include "Math_Utility.h"
#include "GameInstance.h"

CMath_Utility::CMath_Utility()
{
}

bool CMath_Utility::Picking(CVIBuffer* pVIBuffer, CTransform* pTransform,  _float4* pOut)
{
	RAY _Ray = Get_MouseRayInWorldSpace();

	_fmatrix WorldMatrix = pTransform->Get_WorldMatrix();                 // 월드 메트릭스 가져옴
	_fmatrix LocalMatrix = XMMatrixInverse(nullptr, WorldMatrix);    // 열행렬 곱해 로컬스페이스로 감

	_fvector			vRayPos = XMVector3TransformCoord(XMLoadFloat3(&_Ray.vPos), LocalMatrix);	  // Ray위치백터 -> 로컬스페이스로보냄 
	_fvector			vRayDir = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&_Ray.vDir), LocalMatrix));     // Ray방향백터 -> 로컬스페이스로보냄 & Ray방향백터 정규화     

	_uint iNumFaces = pVIBuffer->Get_NumPrimitive();                           // VIBuffer에있는 폴리곤 수 가져옴
	const _float3* pVerticesPos = pVIBuffer->Get_VerticesPos();                // VIBuffer에있는 버텍스 위치 가져옴

	_float fDist;

	for (_uint i = 0; i < iNumFaces; ++i)                                     // 폴리곤 수만큼 반복
	{
		_uint3 iIndices = pVIBuffer->Get_Indices(i);                    // 해당 폴리곤 인덱스 가져옴

		_vector		vPickedPos;
		
		

		if (true == TriangleTests::Intersects(vRayPos, vRayDir, XMLoadFloat3( &pVerticesPos[iIndices.ix]), XMLoadFloat3(&pVerticesPos[iIndices.iy]), XMLoadFloat3(&pVerticesPos[iIndices.iz]), fDist))
		{
			if (fDist < _Ray.fLength)
			{
				vPickedPos = vRayPos + vRayDir * fDist;
				_float3 fTemp;
				XMStoreFloat3(&fTemp, XMVector3TransformCoord(vPickedPos, pTransform->Get_WorldMatrix()));

				pOut->x = fTemp.x;
				pOut->y = fTemp.y;
				pOut->z = fTemp.z;
				pOut->w = 1.f;

				return true;
			}

		}
		/*if (true == TriangleTests::Intersects(vRayPos, vRayDir
			, XMLoadFloat3(&pVerticesPos[iIndices.ix]), XMLoadFloat3(&pVerticesPos[iIndices.iy]), XMLoadFloat3(&pVerticesPos[iIndices.iz]), fDist))
		{
			vPickedPos = vRayPos + vRayDir * fDist;
			XMStoreFloat4(pOut, XMVector3TransformCoord(vPickedPos, pTransform->Get_WorldMatrix()));
			return true;
		}*/
	}

	return false;
}

RAY CMath_Utility::Get_MouseRayInWorldSpace()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	D3D11_VIEWPORT   ViewPort;
	_uint			viewportCount = 1;
	ZeroMemory(&ViewPort, sizeof(D3D11_VIEWPORT));
	CONTEXT->RSGetViewports(&viewportCount,&ViewPort);

	POINT  ptMouse;
	GetCursorPos(&ptMouse);
	//ptMouse.x = (ViewPort.Width * 0.5f);
	//ptMouse.y = (ViewPort.Height * 0.5f);
	ScreenToClient(pGameInstance->Get_GraphicDesc().hWnd, &ptMouse);

	
	/* 2. 투영 스페이스 상의 마우스 좌표 구하기 */
	_float3  vProjPos;

	vProjPos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vProjPos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vProjPos.z = 0.0f;

	/* 3.뷰스페이스상의 마우스 좌표를 구하자. */
	_vector	vViewPos;

	_matrix ProjMatrixInv = XMMatrixInverse( nullptr, pGameInstance->Get_Transform(CPipeLine::D3DTS_PROJ));
	vViewPos = XMVector3TransformCoord( XMLoadFloat3(&vProjPos), ProjMatrixInv);

	/* 4.마우스레이와 마우스Pos를구하자.  */
	_vector	vRayDir, vRayPos;

	vRayDir = vViewPos - XMVectorSet(0.f, 0.f, 0.f,1.f);
	vRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	/* 5.월드로가자. */
	_matrix ViewMatrixInv = XMMatrixInverse(nullptr, pGameInstance->Get_Transform(CPipeLine::D3DTS_VIEW));

	RAY MouseRay;

	XMStoreFloat3(&MouseRay.vDir  , XMVector3TransformNormal( vRayDir, ViewMatrixInv));
	XMStoreFloat3(&MouseRay.vPos , XMVector3TransformCoord( vRayPos, ViewMatrixInv));
	MouseRay.fLength = 10000.0f; //무한
	
	RELEASE_INSTANCE(CGameInstance);

	return MouseRay;
}

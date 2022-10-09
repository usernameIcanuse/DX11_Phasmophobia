
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix  g_ViewMatrixInv, g_ProjMatrixInv;
matrix  g_DecalInvWorldMat;

float g_fFar = 300.f;

texture2D	g_DiffuseTexture;
texture2D	g_DelpthTexture;

sampler DefaultSampler = sampler_state 
{		
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;

	return Out;	
}

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{	
	vector		vColor : SV_TARGET0;	
};

float2 GetUVPos(float4 vWVP)
{
	float4 vProjPos = vWVP;

	float2 UV = vProjPos.xy / vProjPos.w;
	UV = UV * float2(0.5f, -0.5f) + 0.5f;

	return UV;
}


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 PixelUV = GetUVPos(In.vPosition);
	vector vPixelDepth = g_DelpthTexture.Sample(DefaultSampler, PixelUV);
	float  fViewZ = vPixelDepth.y * g_fFar;

	vector			vWorldPos;

	/* 투영스페이스 상의 위치르 ㄹ구한다. */
	/* 뷰스페이스 상 * 투영행렬 / w 까지 위치를 구한다. */
	vWorldPos.x = PixelUV.x * 2.f - 1.f;
	vWorldPos.y = PixelUV.y * -2.f + 1.f;
	vWorldPos.z = vPixelDepth.x;
	vWorldPos.w = 1.0f;

	/* 뷰스페이스 상 * 투영행렬까지 곱해놓은 위치를 구한다. */
	vWorldPos *= fViewZ;

	/* 뷰스페이스 상  위치를 구한다. */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);


	/* 월드페이스 상  위치를 구한다. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	/*데칼 로컬로 옮겨줌*/
	vector vDecalLocalPos = mul(vWorldPos, g_DecalInvWorldMat);
	//데칼 큐브는 계산편의를 위해서 로컬 길이를 1,1,1로 해놓음 
	if (vDecalLocalPos.x < -0.5f || vDecalLocalPos.x > 0.5f || vDecalLocalPos.y < -0.5f || vDecalLocalPos.y > 0.5f ||
		vDecalLocalPos.z < -0.5f || vDecalLocalPos.z > 0.5f)
		discard;
	
	float fDecalUV = vDecalLocalPos.xy + 0.5f;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, fDecalUV);


	return Out;	
}


technique11 DefaultTechnique
{

	pass Default_Alpha
	{	
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();

	}

}
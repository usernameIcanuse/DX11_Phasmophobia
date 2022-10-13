
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix  g_ViewMatrixInv, g_ProjMatrixInv;
matrix  g_DecalInvWorldMat;

float g_fFar = 300.f;

texture2D	g_DiffuseTexture;
texture2D	g_DepthTexture;

sampler DefaultSampler = sampler_state 
{		
	filter = min_mag_mip_linear;

};

struct VS_IN_DECAL
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT_DECAL
{
	float4		vPosition : SV_POSITION;
	float4		vProjPos :TEXCOORD0;
};

VS_OUT_DECAL VS_MAIN_DECAL(VS_IN_DECAL In)
{
	VS_OUT_DECAL		Out = (VS_OUT_DECAL)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;
	

	return Out;	
}

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )

struct PS_IN_DECAL
{
	float4		vPosition : SV_POSITION;
	float4		vProjPos :TEXCOORD0;

};

struct PS_OUT_DECAL
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


PS_OUT_DECAL PS_MAIN_DECAL(PS_IN_DECAL In)
{
	PS_OUT_DECAL		Out = (PS_OUT_DECAL)0;

	float2 PixelUV = GetUVPos(In.vProjPos);
	vector vPixelDepth = g_DepthTexture.Sample(DefaultSampler, PixelUV);
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
	{ //discard를 하면 현재 그리려는 화면 픽셀 색이 있어도 그냥 0,0,0,0으로 만듦
		discard;
	}
	
	float2 fDecalUV = vDecalLocalPos.xy + 0.5f;
	fDecalUV.y = 1.f - fDecalUV.y;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, fDecalUV);
	Out.vColor.x = 220.f;
	Out.vColor.y = 255.f;
	Out.vColor.z = 0.f;

	if (Out.vColor.a < 0.4f)
		discard;
	

	return Out;	
}

DepthStencilState DSS_DepthFalse_StencilWrite
{
	DepthEnable = false;
	DepthWriteMask = zero;

	StencilEnable = true;
	StencilWriteMask = 0xff;

	//FrontFaceStencilRef = 1;

	FrontFaceStencilFunc = always;
	FrontFaceStencilPass = Replace;
};//

technique11 DefaultTechnique
{
	pass Default_Alpha
	{	
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthFalse_StencilWrite, 1);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_DECAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DECAL();

	}

}
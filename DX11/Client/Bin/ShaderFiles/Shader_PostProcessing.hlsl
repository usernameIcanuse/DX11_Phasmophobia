
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;



float2 g_ScanLineJitter = float2(0.2f,0.9f); // (displacement, threshold)
float2 g_VerticalJump = float2(0.5f,0.1f);   // (amount, time)
float g_HorizontalShake = 0.2f;
float2 g_ColorDrift = float2(0.3f,0.1f);     // (amount, time)
float g_Time = 3.f;


float g_fFar = 300.f;

texture2D	g_Texture;

sampler DefaultSampler = sampler_state 
{		
	filter = min_mag_mip_linear;

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
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
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


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	
	return Out;	
}

float nrand(float x, float y)
{
	return frac(sin(dot(float2(x, y), float2(12.9898, 78.233))) * 43758.5453);
}

PS_OUT PS_MAIN_GLITCH(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float u = In.vTexUV.x;
	float v = In.vTexUV.y;

	// Scan line jitter
	float jitter = nrand(v,g_Time) * 2 - 1;
	jitter *= step(g_ScanLineJitter.y, abs(jitter)) * g_ScanLineJitter.x;

	// Vertical jump
	float jump = lerp(v, frac(v + nrand(g_VerticalJump.y,g_Time) * 2 - 1), g_VerticalJump.x);

	// Horizontal shake
	float shake=(nrand(g_Time,2) - 0.5) * g_HorizontalShake;

	// Color drift
	//float drift = sin(jump*0.5f + g_ColorDrift.y) * g_ColorDrift.x;

	//vector src1 = g_Texture.Sample(DefaultSampler, frac(float2(u + jitter + shake, jump)));
	//vector src2 = g_Texture.Sample(DefaultSampler, frac(float2(u + jitter +  shake + drift, jump)));

	//Out.vColor = vector(src1.r, src2.g, src1.b, 1);
	Out.vColor = g_Texture.Sample(DefaultSampler, frac(float2(u + jitter + shake, jump)));
	return Out;

}



technique11 DefaultTechnique
{
	pass Default
	{	
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 1);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();

	}

	pass Glitch
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 1);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_GLITCH();
	}

}
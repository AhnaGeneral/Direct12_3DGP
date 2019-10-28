struct MATERIAL
{
	float4					m_cAmbient;
	float4					m_cDiffuse;
	float4					m_cSpecular; //a = power
	float4					m_cEmissive;
};

cbuffer cbCameraInfo : register(b1)
{
	matrix					gmtxView : packoffset(c0);
	matrix					gmtxProjection : packoffset(c4);
	float3					gvCameraPosition : packoffset(c8);
};

cbuffer cbGameObjectInfo : register(b2)
{
	matrix					gmtxGameObject : packoffset(c0);
	MATERIAL				gMaterial : packoffset(c4);
};

#include "Light.hlsl"
//[ Lighting, Texture Shader Code ]=====================================================
Texture2D gtxtTexture : register(t0);
SamplerState gSamplerState : register(s0);

struct VS_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};
struct VS_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 uv : TEXCOORD;
};

VS_LIGHTING_OUTPUT VSLighting(VS_LIGHTING_INPUT input)
{
	VS_LIGHTING_OUTPUT output;

	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.uv = input.uv; 

	return(output);
}

float4 PSLighting(VS_LIGHTING_OUTPUT input) : SV_TARGET
{
	input.normalW = normalize(input.normalW);
	float4 color = Lighting(input.positionW, input.normalW);
	float4 cColor = gtxtTexture.Sample(gSamplerState, input.uv);
	
	cColor = lerp(color, cColor, 0.5f);
	return(cColor);
}

Texture2D gtxTerrainBaseTexture : register(t1); 
Texture2D gtxTerrainDetailTexture: register(t2);  

struct VS_TERRAIN_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
};

struct VS_TERRAIN_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
};

VS_TERRAIN_OUTPUT VSTerrain(VS_TERRAIN_INPUT input)
{
	VS_TERRAIN_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.color = input.color;
	output.uv0 = input.uv0;
	output.uv1 = input.uv1;

	return(output);
}

float4 PSTerrain(VS_TERRAIN_OUTPUT input) : SV_TARGET
{
	float4 cBaseTexColor = gtxTerrainBaseTexture.Sample(gSamplerState, input.uv0);
	float4 cDetailTexColor = gtxTerrainDetailTexture.Sample(gSamplerState, input.uv1);
	float4 cColor = input.color * saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));

	//float4 cColor = float4(1,0,0,1);
	return(cColor);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Texture2D gtxWaterTexture: register(t3);

struct VS_WATER_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_WATER_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_WATER_OUTPUT VSWater(VS_WATER_INPUT input)
{
	VS_WATER_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);
}

float4 PSWater(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxWaterTexture.Sample(gSamplerState, input.uv);

	return(cColor);
}



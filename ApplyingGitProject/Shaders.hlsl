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
SamplerState gWrapSamplerState : register(s0);
SamplerState gClampSamplerState : register(s1);

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
	float4 cColor = gtxtTexture.Sample(gWrapSamplerState, input.uv);
	
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
	float4 cBaseTexColor = gtxTerrainBaseTexture.Sample(gWrapSamplerState, input.uv0);
	float4 cDetailTexColor = gtxTerrainDetailTexture.Sample(gWrapSamplerState, input.uv1);
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

float4 PSWater(VS_WATER_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxWaterTexture.Sample(gWrapSamplerState, input.uv) *0.6f ;
	//float4 cColor = (1.0f, 1.0f, 0.0f, 1.0f);
	return(cColor);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Texture2D gtxtBillboardTextures: register(t4);


struct VS_BILLBOARD_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 instancePosition : INSTANCEPOSITION;
	float4 billboardInfo : BILLBOARDINFO; //(cx, cy, type, texture)
};

struct VS_BILLBOARD_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	int textureID : TEXTUREID;
};

VS_BILLBOARD_OUTPUT VSBillboard(VS_BILLBOARD_INPUT input)
{
	VS_BILLBOARD_OUTPUT output;

	if (input.position.x < 0.0f) input.position.x = -(input.billboardInfo.x * 0.5f);
	else if (input.position.x > 0.0f) input.position.x = (input.billboardInfo.x * 0.5f);
	if (input.position.y < 0.0f) input.position.y = -(input.billboardInfo.y * 0.5f);
	else if (input.position.y > 0.0f) input.position.y = (input.billboardInfo.y * 0.5f);

	float3 f3Look = normalize(gvCameraPosition - input.instancePosition);
	float3 f3Up = float3(0.0f, 1.0f, 0.0f);
	float3 f3Right = normalize(cross(f3Up, f3Look));

	matrix mtxWorld;
	mtxWorld[0] = float4(f3Right, 0.0f);
	mtxWorld[1] = float4(f3Up, 0.0f);
	mtxWorld[2] = float4(f3Look, 0.0f);
	mtxWorld[3] = float4(input.instancePosition, 1.0f);

	output.position = mul(mul(mul(float4(input.position, 1.0f), mtxWorld), gmtxView), gmtxProjection);

	output.uv = input.uv;
	output.textureID = (int)input.billboardInfo.w - 1;

	return(output);
}

float4 PSBillboard(VS_BILLBOARD_OUTPUT input) : SV_TARGET
{
    float4 cColor = gtxtBillboardTextures.Sample(gWrapSamplerState, input.uv);
    //float4 cColor = (1.0f, 1.0f, 0.0f, 1.0f);
	clip(cColor.a - 0.5f);
	return(cColor);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
Texture2D gtxStartViewTexture: register(t5);

struct VS_STARTVIEW_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_STARTVIEW_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_STARTVIEW_OUTPUT VSStartboard(VS_STARTVIEW_INPUT input)
{
	VS_STARTVIEW_OUTPUT output;

	output.position = float4(input.position, 1.0f);
	output.uv = input.uv;

	return(output);
}

float4 PSStartboard(VS_STARTVIEW_OUTPUT output) : SV_TARGET
{
	float4 cColor = gtxStartViewTexture.Sample(gWrapSamplerState, output.uv);
	//float4 cColor = (1.0f, 1.0f, 0.0f, 1.0f);
	//clip(cColor.a - 0.5f);
	return(cColor);
}




cbuffer cbPlayerInfo : register(b0)
{
	matrix		gmtxPlayerWorld : packoffset(c0);
};

cbuffer cbCameraInfo : register(b1)
{
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
};

cbuffer cbGameObjectInfo : register(b2)
{
	matrix		gmtxGameObject : packoffset(c0);
};

/*
cbuffer cbTerrainInfo : register(b3)
{
	float3		gf3TerrainScale : packoffset(c0);
	float2		gf2TerrainHeightMapSize : packoffset(c1);
};
*/
static float3 gf3TerrainScale = float3(8.0f, 2.0f, 8.0f);
static float2 gf2TerrainHeightMapSize = float2(257.0f, 257.0f);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_DIFFUSED_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VS_DIFFUSED_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VS_DIFFUSED_OUTPUT VSPlayer(VS_DIFFUSED_INPUT input)
{
	VS_DIFFUSED_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxPlayerWorld), gmtxView), gmtxProjection);
	output.color = input.color;

	return(output);
}

float4 PSPlayer(VS_DIFFUSED_OUTPUT input) : SV_TARGET
{
	return(input.color);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtTexture : register(t0);
SamplerState gSamplerState : register(s0);

struct VS_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_TEXTURED_OUTPUT VSTextured(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);
}

float4 PSTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtTexture.Sample(gSamplerState, input.uv);

	return(cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D<float4> gtxtTerrainBaseTexture : register(t1);

//#define _WITH_HEIGHT_MAP_TEXTURE

#ifdef _WITH_HEIGHT_MAP_TEXTURE
Texture2D<float> gtxtTerrainHeightMapTexture : register(t2);
#else
Buffer<float> gfTerrainHeightMapBuffer : register(t2);
#endif

struct VS_TERRAIN_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
};

struct VS_TERRAIN_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
};

#ifdef _WITH_HEIGHT_MAP_TEXTURE
float GetTerrainHeight(float fx, float fz)
{
	if ((fx < 0.0f) || (fz < 0.0f) || (fx >= gf2TerrainHeightMapSize.x) || (fz >= gf2TerrainHeightMapSize.y)) return(0.0f);

	int x = (int)fx;
	int z = (int)fz;
	float fxPercent = fx - x;
	float fzPercent = fz - z;
	bool bReverseQuad = ((z % 2) != 0);

	float fBottomLeft = (float)gtxtTerrainHeightMapTexture.Load(float3(x, z, 0));
	float fBottomRight = (float)gtxtTerrainHeightMapTexture.Load(float3((x + 1), z, 0));
	float fTopLeft = (float)gtxtTerrainHeightMapTexture.Load(float3(x, (z + 1), 0));
	float fTopRight = (float)gtxtTerrainHeightMapTexture.Load(float3((x + 1), (z + 1), 0));

	if (bReverseQuad)
	{
		if (fzPercent >= fxPercent)
			fBottomRight = fBottomLeft + (fTopRight - fTopLeft);
		else
			fTopLeft = fTopRight + (fBottomLeft - fBottomRight);
	}
	else
	{
		if (fzPercent < (1.0f - fxPercent))
			fTopRight = fTopLeft + (fBottomRight - fBottomLeft);
		else
			fBottomLeft = fTopLeft + (fBottomRight - fTopRight);
	}
	float fTopHeight = fTopLeft * (1 - fxPercent) + fTopRight * fxPercent;
	float fBottomHeight = fBottomLeft * (1 - fxPercent) + fBottomRight * fxPercent;
	float fHeight = fBottomHeight * (1 - fzPercent) + fTopHeight * fzPercent;

	return(fHeight);
}
#endif

VS_TERRAIN_OUTPUT VSTerrain(VS_TERRAIN_INPUT input)
{
	VS_TERRAIN_OUTPUT output;

	float x = input.position.x / gf3TerrainScale.x;
	float z = input.position.z / gf3TerrainScale.z;
#ifdef _WITH_HEIGHT_MAP_TEXTURE
	input.position.y = GetTerrainHeight(x, z) * 255.0f * gf3TerrainScale.y;
//	input.position.y = gtxtTerrainHeightMapTexture.Load(float3(x, z, 0)) * 255.0f * gf3TerrainScale.y;
#else
	input.position.y = gfTerrainHeightMapBuffer.Load(int(x) + int(z * gf2TerrainHeightMapSize.x)) * 255.0f * gf3TerrainScale.y;
#endif

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.color = input.color;
	output.uv = input.uv;

	return(output);
}

float4 PSTerrain(VS_TERRAIN_OUTPUT input) : SV_TARGET
{
	float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gSamplerState, input.uv);
	float4 cColor = input.color * cBaseTexColor;

	return(cColor);
}



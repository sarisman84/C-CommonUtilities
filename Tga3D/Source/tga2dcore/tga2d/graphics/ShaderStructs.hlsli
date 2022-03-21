// VERY useful resource:
// https://www.programmersought.com/article/41834061891/

// duplicated from Common.hlsli, should be removed
cbuffer CommonBuffer : register(b0)
{
	float4 myResolution; //myResolution.x = screen width, myResolution.y = screen height, myResolution.z = unset, myResolution.w = unset
	float4 myTimings; //myTimings.x = totaltime, myTimings.y = delta time, myTimings.z = unset, myTimings.w = unset

	float4x4 myToCamera;
	float4x4 myToProjection;
	float4 myCameraPosition;
	float myNearPlane;
	float myFarPlane;
	float myUnused0;
	float myUnused1;
};

#define MAX_ANIMATION_BONES 64 

cbuffer BoneBuffer : register(b3)
{
	float4x4 myBones[MAX_ANIMATION_BONES];
};

cbuffer ObjectBuffer : register(b2)
{
	float4x4 OB_ToWorld;
}

#define NUMBER_OF_LIGHTS_ALLOWED 8 

cbuffer LightConstantBufferData : register(b1)
{
	struct PointLightData
	{
		float4 Position;
		float4 Color;
		float Range;
		float3 garbage; // Padding, don't use arrays!
	} myPointLights[NUMBER_OF_LIGHTS_ALLOWED];

	uint myNumberOfLights;
	int myNumEnvMapMipLevels;
	float garbage0;
	float garbage1;

	float4 myAmbientLightColorAndIntensity;
	float4 myDirectionalLightDirection;
	float4 myDirectionalLightColorAndIntensity;
};

struct VertexInput
{
	float4 myPosition	:	POSITION;
	float4 myVxColor	:	COLOR0;
	float4 myVxColor2	:	COLOR1;
	float4 myVxColor3	:	COLOR2;
	float4 myVxColor4	:	COLOR3;
	float2 myUV			:	UV0;
	float2 myUV2		:	UV1;
	float2 myUV3		:	UV2;
	float2 myUV4		:	UV3;
	float3 myNormal		:	NORMAL;
	float3 myTangent	:	TANGENT;
	float3 myBiNormal	:	BINORMAL;
	float4 myBones      :   BONES;
	float4 myWeights    :   WEIGHTS;
};

struct VertexToPixel
{
	float4 myPosition		:	SV_POSITION;
	float4 myWorldPosition	:	POSITION;
	float myDepth			:	DEPTH;
	float4 myVxColor		:	COLOR0;
	float4 myVxColor2		:	COLOR1;
	float4 myVxColor3		:	COLOR2;
	float4 myVxColor4		:	COLOR3;
	float2 myUV				:	UV0;
	float2 myUV2			:	UV1;
	float2 myUV3			:	UV2;
	float2 myUV4			:	UV3;
	float3 myNormal			:	NORMAL;
	float3 myTangent		:	TANGENT;
	float3 myBiNormal		:	BINORMAL;
};

struct PixelOutput
{
	float4 myColor		:	SV_TARGET;
};

TextureCube environmentTexture : register(t0);

Texture2D albedoTexture		: register(t1);
Texture2D normalTexture		: register(t2);
Texture2D materialTexture	: register(t3);

SamplerState defaultSampler : register(s0);
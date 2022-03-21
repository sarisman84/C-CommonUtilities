#include "../render/common.hlsli"

struct VertexInputType
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

cbuffer BoneBuffer : register(b3)
{
	float4x4 myBones[MAX_ANIMATION_BONES];
};

cbuffer ModelObjectBuffer : register(b2) 
{
	float4x4 myObToWorld;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	float2 Resolution = myResolution.xy;
	float ratio = Resolution.y / Resolution.x;
		
	float4 pos = input.myPosition;
	float4 skinnedPos = 0;
	uint iBone = 0;
	float fWeight = 0;
	
	iBone = input.myBones.x;
	fWeight = input.myWeights.x;
	
	// Bone 0
	iBone = input.myBones.x;
	fWeight = input.myWeights.x;
	skinnedPos += fWeight * mul(pos, myBones[iBone]);

	// Bone 1
	iBone = input.myBones.y;
	fWeight = input.myWeights.y;
	skinnedPos += fWeight * mul(pos, myBones[iBone]);
	
	// Bone 2
	iBone = input.myBones.z;
	fWeight = input.myWeights.z;
	skinnedPos += fWeight * mul(pos, myBones[iBone]);

	// Bone 3
	iBone = input.myBones.w;
	fWeight = input.myWeights.w;
	skinnedPos += fWeight * mul(pos, myBones[iBone]);
	
	float4 vertexWorldPos = mul(myObToWorld, skinnedPos);
	float4 vertexViewPos = mul(myToCamera, vertexWorldPos);
	output.worldPosition = vertexWorldPos;
	output.position = mul(myToProjection, vertexViewPos);

	output.tex = input.myUV;
	output.color = 1; 

    return output;  
}
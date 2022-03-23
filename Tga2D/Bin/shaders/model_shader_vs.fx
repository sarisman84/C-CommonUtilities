#include "common.fx"

struct VertexInputType
{
    float4 position : POSITION; 
	float4 Bones : BONES;  
	float4 Weights : WEIGHTS;  
    float2 tex : UV;
};

cbuffer BoneBuffer : register(b5)
{
	float4x4 myBones[BONES_MAX];
};

cbuffer ModelObjectBuffer : register(b4) 
{
	float4 myWorldPosition;
	float4 myOffsetRotation;
	float2 myOffsetPosition;
	float2 myScale;
	float2 myRotation;
	float2 myDummy;
};


PixelInputType VShader(VertexInputType input)
{
    PixelInputType output;
	
	
	float2 Resolution = myResolution.xy;
	float ratio = Resolution.y / Resolution.x;
		
	float2x2 theRotation = ComputeRotation(-myOffsetRotation.x);
	float4 pos = input.position;
	pos.xy = mul(pos.xy, theRotation);
	pos.xy += myOffsetPosition;
	output.position.w = 1;
	float4 skinnedPos = 0;
	uint iBone = 0;
	float fWeight = 0;
	
	iBone = input.Bones.x;
	fWeight = input.Weights.x;
	
	// Bone 0
	iBone = input.Bones.x;
	fWeight = input.Weights.x;
	skinnedPos += fWeight * mul(pos, myBones[iBone]);

	// Bone 1
	iBone = input.Bones.y;
	fWeight = input.Weights.y;
	skinnedPos += fWeight * mul(pos, myBones[iBone]);
	
	// Bone 2
	iBone = input.Bones.z;
	fWeight = input.Weights.z;
	skinnedPos += fWeight * mul(pos, myBones[iBone]);

	// Bone 3
	iBone = input.Bones.w;
	fWeight = input.Weights.w;
	skinnedPos += fWeight * mul(pos, myBones[iBone]);
	
	output.position.x = skinnedPos.x;
	output.position.y = skinnedPos.y;
	
	float2x2 objectRot = ComputeRotation(myRotation.x);
	output.position.xy = mul(output.position.xy, objectRot);
	
	output.position.x *= ratio;

	output.position.xy *= myScale.xy ;
	output.position.x += (myWorldPosition.x * 2) - 1;
	output.position.y += 1.0f - (myWorldPosition.y * 2);

		
	output.position.z = (skinnedPos.z + 1) / 2;

	output.tex = input.tex;
	output.color = 1; 
	output.textureMappingData = 0;
	
    return output;  
}
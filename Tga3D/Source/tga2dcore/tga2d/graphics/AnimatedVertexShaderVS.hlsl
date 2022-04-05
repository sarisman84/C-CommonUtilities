#include "ShaderStructs.hlsli"

// This gets Log Depth from worldPosition
float GetLogDepth(float4 worldPosition)
{
	float4 worldToView = mul(myToCamera, worldPosition);
	float4 viewToProj = mul(myToProjection, worldToView);
	return viewToProj.z / viewToProj.w;
}

float GetLinDepth(float4 worldPosition)
{
	float logDepth = GetLogDepth(worldPosition);
	return myNearPlane / (myFarPlane - logDepth * (myFarPlane - myNearPlane));
}

// Converts Log Depth to Lin Depth
float LogDepthToLinDepth(float depth)
{
	return myNearPlane / (myFarPlane - depth * (myFarPlane - myNearPlane));
}

// No idea why this is a thing? We have the world pos, so why read it?
//GetDepth(worldPositionTexture.Sample(defaultSampler, GetScreenCoords(input.myWorldPosition)).xyzw);

float3x3 invertMatrix(float3x3 m)
{
	// computes the inverse of a matrix m
	float det = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
		m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
		m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

	float invdet = 1 / det;

	float3x3 minv; // inverse of matrix m
	minv[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * invdet;
	minv[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invdet;
	minv[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invdet;
	minv[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invdet;
	minv[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invdet;
	minv[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) * invdet;
	minv[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * invdet;
	minv[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) * invdet;
	minv[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * invdet;

	return minv;
}

VertexToPixel main(VertexInput input)
{
	VertexToPixel result;

	float4 pos = input.myPosition;
	float4x4 skinnedMatrix = 0;
	uint iBone = 0;
	float fWeight = 0;

	iBone = input.myBones.x;
	fWeight = input.myWeights.x;

	// Bone 0
	iBone = input.myBones.x;
	fWeight = input.myWeights.x;
	skinnedMatrix += fWeight * myBones[iBone];

	// Bone 1
	iBone = input.myBones.y;
	fWeight = input.myWeights.y;
	skinnedMatrix += fWeight * myBones[iBone];

	// Bone 2
	iBone = input.myBones.z;
	fWeight = input.myWeights.z;
	skinnedMatrix += fWeight * myBones[iBone];

	// Bone 3
	iBone = input.myBones.w;
	fWeight = input.myWeights.w;
	skinnedMatrix += fWeight * myBones[iBone];

	float4 vertexWorldPos = mul(OB_ToWorld, mul(pos, skinnedMatrix));
	float4 vertexViewPos = mul(myToCamera, vertexWorldPos);
	float4 vertexProjectionPos = mul(myToProjection, vertexViewPos);

	float3x3 skinnedRotation = (float3x3)skinnedMatrix;
	float3x3 toWorldRotation = (float3x3)OB_ToWorld;
	//float3 vertexWorldNormal = mul(toWorldRotation, input.myNormal);

	float3 vertexWorldBiNormal = mul(toWorldRotation, mul(input.myBiNormal, skinnedRotation));
	float3 vertexWorldTangent = mul(toWorldRotation, mul(input.myTangent, skinnedRotation));
	float3 vertexWorldNormal = mul(transpose(invertMatrix(toWorldRotation)), mul(input.myNormal, transpose(invertMatrix(skinnedRotation))));

	result.myPosition = vertexProjectionPos;
	result.myWorldPosition = vertexWorldPos;
	result.myVxColor = input.myVxColor;
	result.myVxColor2 = input.myVxColor2;
	result.myVxColor3 = input.myVxColor3;
	result.myVxColor4 = input.myVxColor4;

	result.myUV = input.myUV;
	result.myUV2 = input.myUV2;
	result.myUV3 = input.myUV3;
	result.myUV4 = input.myUV4;

	result.myNormal = vertexWorldNormal;
	result.myBiNormal = vertexWorldBiNormal;
	result.myTangent = vertexWorldTangent;
	
	result.myDepth = GetLinDepth(vertexWorldPos);
	//result.myDepth = GetLogDepth(vertexWorldPos);
	//result.myDepth = LogDepthToLinDepth(vertexWorldPos.z / vertexWorldPos.w);

	//float4x4 camViewProj = FB_ToCamera * FB_ToProjection;
	//float4 temp = mul(vertexWorldPos, camViewProj);
	//result.myDepth = LogDepthToLinDepth(((temp.w - FB_NearPlane) / FB_FarPlane - FB_NearPlane) * -1);

	//result.myVxColor2 = mul(camViewProj, vertexWorldPos);
	
	return result;
}


// Get screen texture coordinates from world position?
float2 GetScreenCoords(float4 worldPosition)
{
	float4 worldToView = mul(myToCamera, worldPosition);
	float4 viewToProj = mul(myToProjection, worldToView);

	float2 projectedTextureCoords;
	projectedTextureCoords.x = viewToProj.x / viewToProj.w / 2.0f + 0.5f;
	projectedTextureCoords.y = viewToProj.y / viewToProj.w / 2.0f + 0.5f;

	return projectedTextureCoords;
}
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

VertexToPixel main(VertexInput input)
{
	VertexToPixel result;

	float4 vertexObjectPos = input.myPosition;
	float4 vertexWorldPos = mul(OB_ToWorld, vertexObjectPos);
	float4 vertexViewPos = mul(myToCamera, vertexWorldPos);
	float4 vertexProjectionPos = mul(myToProjection, vertexViewPos);

	float3x3 toWorldRotation = (float3x3)OB_ToWorld;
	float3 vertexWorldNormal = mul(toWorldRotation, input.myNormal);
	float3 vertexWorldBiNormal = mul(toWorldRotation, input.myBiNormal);
	float3 vertexWorldTangent = mul(toWorldRotation, input.myTangent);
	
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
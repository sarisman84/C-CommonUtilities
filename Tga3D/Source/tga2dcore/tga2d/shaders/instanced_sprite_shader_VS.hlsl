#include "../render/common.hlsli"

struct VertexInputType
{
    float4 position : POSITION;
	uint4 myVertexIndex : TEXCOORD0;
	float4 instanceTransform0 : TEXCOORD1;
	float4 instanceTransform1 : TEXCOORD2;
	float4 instanceTransform2 : TEXCOORD3;
	float4 instanceTransform3 : TEXCOORD4;
	float4 instanceColor : TEXCOORD5;
	float4 instanceUV : TEXCOORD6;
	float4 myUVRect : TEXCOORD7;
	float4 myAspectRatio : TEXCOORD8;
};

static uint2 textureRectLookup[6] =
	{
		uint2(0, 3),
		uint2(0, 1),
		uint2(2, 3),
		uint2(2, 1),
		uint2(2, 3),
		uint2(0, 1),
	};
	
float2 GetUVRect(float4 aRect, uint aIndex)
{
	uint2 theLookuped = textureRectLookup[aIndex];
	return float2(aRect[theLookuped.x], aRect[theLookuped.y]);
}

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	float2 Resolution = myResolution.xy;
	float ratio = Resolution.y / Resolution.x;

	float4x4 transform = float4x4(input.instanceTransform0, input.instanceTransform1, input.instanceTransform2, input.instanceTransform3);

	float4 vertexWorldPos = mul(input.position, transform);
	float4 vertexViewPos = mul(myToCamera, vertexWorldPos);
	float4 vertexProjectionPos = mul(myToProjection, vertexViewPos);

	output.position = vertexProjectionPos;
	output.worldPosition = vertexWorldPos;

	float2 textureRect = GetUVRect(input.myUVRect, input.myVertexIndex.x);
	output.tex = input.instanceUV.xy + (textureRect * input.instanceUV.zw);
	
	output.color = input.instanceColor;	
	output.aspectRatio = input.myAspectRatio;

    return output;
}
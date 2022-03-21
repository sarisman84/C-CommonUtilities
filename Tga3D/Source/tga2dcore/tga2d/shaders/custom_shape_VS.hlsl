#include "../render/common.hlsli"

struct VertexInputType
{
    float4 position : POSITION;
    float4 color : TEXCOORD0;
	float2 tex : TEXCOORD1;
};

struct PixelInputTypeCustom
{
    float4 position : SV_POSITION;
	float4 color : TEXCOORD0;
	float textureMappingData : TEXCOORD1;
	float2 tex : TEXCOORD2;
};

PixelInputTypeCustom main(VertexInputType input)
{
    PixelInputTypeCustom output;
	

    // Change the position vector to be 4 units for proper matrix calculations.
	
    input.position.w = 1.0f;	
	
			
	float2x2 theRotation = ComputeRotation(myRotationAndSizeCustom.x);
	input.position.xy = mul(input.position.xy, theRotation);
	
	
	input.position.x *= myRotationAndSizeCustom.y;
	input.position.y *= myRotationAndSizeCustom.z;
	
			
    // Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = input.position;
	output.position.xy +=myPositionCustom.xy;
	output.tex = input.tex;
    
	output.color = input.color;
	output.textureMappingData = myRotationAndSizeCustom.w;
	
    return output;
}
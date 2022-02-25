#include "common.fx"

SamplerState SampleType;

Texture2D shaderTextures[2] : register( t1 );
Texture2D shaderTextureCustom : register( t4 );

#define DIFFUSE_MAP 0

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 color : TEXCOORD1;
	float textureMappingData : TEXCOORD2;
};


float4 PShader(PixelInputType input) : SV_TARGET
{
	float2 fragCoord = input.tex.xy * float2(1280, 720);
	float PI = 3.14159265;
	float time = myTimings.x *0.2;

    float color1, color2, color;
	
	color1 = (sin(dot(fragCoord,float2(sin(time*3.0),cos(time*3.0)))*0.02+time*3.0)+1.0)/2.0;
	
	float2 center = float2(1280.0/2.0, 720.0/2.0) + float2(1280.0/2.0*sin(-time*3.0),720.0/2.0*cos(-time*3.0));
	
	color2 = (cos(length(fragCoord - center)*0.03)+1.0)/2.0;
	
	color = (color1+ color2)/2.0;

	float red	= (cos(PI*color/0.5+time*3.0)+1.0)/2.0;
	float green	= (sin(PI*color/0.5+time*3.0)+1.0)/2.0;
	float blue	= (sin(+time*3.0)+1.0)/2.0;
	
	float4 Diffuse = shaderTextureCustom.Sample(SampleType, input.tex);
    Diffuse.xyz = float3(red, green, blue);

	return Diffuse;
}
#include "../render/common.hlsli"

#define DIFFUSE_MAP 0
#define NORMAL_MAP 1

SamplerState SampleType;
Texture2D shaderTextures[2] : register( t4 );

float4 main(PixelInputType input) : SV_TARGET
{
	float4 Diffuse = shaderTextures[DIFFUSE_MAP].Sample(SampleType, input.tex) * input.color;
	
	#ifdef USE_NOISE
	float4 noise = myNoiseTexture.Sample(SampleType, input.tex); 
	#endif
	
	float3 AmbientColor = Diffuse.xyz * myAmbientLightColorAndIntensity.xyz;
	float3 DirectColor = float3(0, 0, 0);
	
	if (Diffuse.w < 0.5)
	{
		discard;
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return float4(AmbientColor + DirectColor, Diffuse.w);
}
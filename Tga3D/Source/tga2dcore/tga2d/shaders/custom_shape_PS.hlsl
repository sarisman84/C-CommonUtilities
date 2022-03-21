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

Texture2D shaderTexture : register(t0);
SamplerState SampleType;

#ifdef USE_LIGHTS
float4 CalculateLight(PixelInputTypeCustom input)
{
	float4 Diffuse = input.color;
	float3 AmbientColor = Diffuse.xyz * myAmbientLightColorAndIntensity.xyz;
	float3 DirectColor = float3(0, 0, 0);

	float2 Resolution = myResolution.xy;
	for (unsigned int index = 0; index < myNumberOfLights; index++)
	{
		float Intensity = myPointLights[index].Color.w;
		float Falloff = myPointLights[index].Range;

		float3 difference = float3((myPointLights[index].Position.xyz - input.position.xyz));
		float distance = length(difference); 

		float linearAttenuation = saturate((Falloff - distance) / Falloff);
		float physicalAttenuation = 1.0f/(1.0f + distance);
		float totalAttenuation = linearAttenuation * physicalAttenuation;

		float3 direction = normalize(float3(difference.xy, (Falloff / 2)));
		DirectColor += Diffuse.xyz * Intensity * totalAttenuation * myPointLights[index].Color.rgb;
	} 	

	return float4(AmbientColor + DirectColor, Diffuse.w);
}
#endif

float4 main(PixelInputTypeCustom  input) : SV_TARGET
{
	float4 Diffuse = shaderTexture.SampleLevel(SampleType, input.tex, 0) * input.color;
	input.color = Diffuse;
	#ifdef USE_LIGHTS
	if(input.textureMappingData > 0.0f)
	{
		return CalculateLight(input);
	}
	#endif
    return input.color;
}
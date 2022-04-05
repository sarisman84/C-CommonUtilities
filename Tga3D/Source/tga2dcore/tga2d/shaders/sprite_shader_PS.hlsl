#include "../render/common.hlsli"

SamplerState SampleType;

#define DIFFUSE_MAP 0
#define NORMAL_MAP 1

Texture2D shaderTextures[2] : register( t1 );

float4 main(PixelInputType input) : SV_TARGET
{
	float4 Diffuse = shaderTextures[DIFFUSE_MAP].Sample(SampleType, input.tex) * input.color;
	
	#ifdef USE_NOISE
	float4 noise = myNoiseTexture.Sample(SampleType, input.tex); 
	#endif
	
	float3 AmbientColor = Diffuse.xyz * myAmbientLightColorAndIntensity.xyz;
	float3 DirectColor = float3(0, 0, 0);
	
	#ifdef USE_LIGHTS
	float3 Normal = normalize((shaderTextures[NORMAL_MAP].Sample(SampleType, input.tex).xyz * 2.0f) - 1.0f);
	float2 Resolution = myResolution.xy;

	for (unsigned int index = 0; index < myNumberOfLights; index++)
	{
		float Intensity = myPointLights[index].Color.w;
		float Falloff = myPointLights[index].Range;

		float3 difference = float3((myPointLights[index].Position.xyz - input.worldPosition.xyz));
		difference.y *= input.aspectRatio.x;

		float distance = length(difference);

		float linearAttenuation = saturate((Falloff - distance) / Falloff);
		float physicalAttenuation = 1.0f / (1.0f + distance);
		float totalAttenuation = linearAttenuation * physicalAttenuation;

		float3 direction = normalize(float3(difference.xy, (Falloff / 2)));


		// need a tbn matrix here...
		float lambert = dot(direction, Normal);
		DirectColor += Diffuse.xyz * Intensity * totalAttenuation * myPointLights[index].Color.rgb * lambert;
	}

	#endif
	
	return float4(AmbientColor + DirectColor, Diffuse.w);
}
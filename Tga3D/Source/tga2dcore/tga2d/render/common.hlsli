#define NUMBER_OF_LIGHTS_ALLOWED 8 
#define MAX_ANIMATION_BONES 64 
#define USE_LIGHTS
#define USE_NOISE

Texture2D myNoiseTexture: register( t0 );
cbuffer CommonBuffer : register(b0) 
{
	float4 myResolution; //myResolution.x = screen width, myResolution.y = screen height, myResolution.z = unset, myResolution.w = unset
	float4 myTimings; //myTimings.x = totaltime, myTimings.y = delta time, myTimings.z = unset, myTimings.w = unset

	float4x4 myToCamera;
	float4x4 myToProjection;
	float4 myCameraPosition;
	float myNearPlane;
	float myFarPlane;
	float myUnused0;
	float myUnused1;

};

cbuffer LightConstantBufferData : register(b1)
{
	struct PointLightData
	{
		float4 Position;
		float4 Color;
		float Range;
		float3 garbage; // Padding, don't use arrays!
	} myPointLights[NUMBER_OF_LIGHTS_ALLOWED];

	uint myNumberOfLights;
	int myNumEnvMapMipLevels;
	float garbage0;
	float garbage1;

	float4 myAmbientLightColorAndIntensity;
	float4 myDirectionalLightDirection;
	float4 myDirectionalLightColorAndIntensity;
};

cbuffer CustomShapeConstantBufferData : register(b2) 
{
	float4 myPositionCustom;
	float4 myRotationAndSizeCustom;
};

struct PixelInputType
{
	float4 position		:	SV_POSITION;
	float4 worldPosition	:	POSITION;
	float2 tex : TEXCOORD0;
	float2 aspectRatio : TEXCOORD1;
	float4 color : TEXCOORD2;
};


float2x2 ComputeRotation(float aRotation) 
{
	float c = cos(aRotation); 
	float s = sin(aRotation);
	return float2x2(c, -s, s, c);
}

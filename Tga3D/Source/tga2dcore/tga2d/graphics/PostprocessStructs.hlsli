struct VertexInput
{
	unsigned int myIndex : SV_VertexID;
};

struct VertexToPixel
{
	float4 myPosition	: SV_POSITION;
	float2 myUV			: UV;
};

struct PixelOutput
{
	float4 myColor		: SV_TARGET;
};

static const float2 Resolution = float2(1280.0f, 720.0f) / 8.0f;
static const float GaussianKernel5[5] =
{ 0.06136f, 0.24477f, 0.38774f, 0.24477f, 0.06136 };

Texture2D FullscreenTexture1 : register(t0);
Texture2D FullscreenTexture2 : register(t1);
Texture2D FullscreenTexture3 : register(t2);
Texture2D FullscreenTexture4 : register(t3);
SamplerState DefaultSampler : register(s0);
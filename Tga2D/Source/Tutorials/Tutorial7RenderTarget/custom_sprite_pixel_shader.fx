#include "common.fx"

SamplerState SampleType;

Texture2D shaderTextures : register( t1 );

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
	float2 p = -1.0 + 2.0 * input.tex.xy ;

    // animation	
	float tz = 0.5 - 0.5*cos(0.225*myTimings.x);
    float zoo = pow( 0.5, 13.0*tz );
	float2 c = float2(-0.05,.6805) + p*zoo;

    // iterate
    float2 z  = float2(0,0);
    float m2 = 0.0;
    float2 dz = float2(0,0);
    for( int i=0; i<256; i++ )
    {
        if( m2>1024.0 ) continue;

        dz = 2.0*float2(z.x*dz.x-z.y*dz.y, z.x*dz.y + z.y*dz.x) + float2(1.0,0.0);
					
        z = float2( z.x*z.x - z.y*z.y, 2.0*z.x*z.y ) + c;
			
        m2 = dot(z,z);
    }

	float d = 0.5*sqrt(dot(z,z)/dot(dz,dz))*log(dot(z,z));

	d = clamp( 8.0*d/zoo, 0.0, 1.0 );
	d = pow( d, 0.25 );
    float3 col = float3( 0, d, d );
    
	float4 Diffuse = shaderTextures.Sample(SampleType, input.tex);
	Diffuse.xyz = col;
	return Diffuse;
}
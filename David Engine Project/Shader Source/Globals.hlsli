#include "Constant Buffer.hlsli"
#include "Structured Buffer.hlsli"
// Texture
Texture2D BindingTexture : register(t0);
Texture2D AtlasTexture : register(t12);
// StructuredBuffer<LightAttribute> lightsAttribute : register(t13);
// StructuredBuffer<Particle> particles : register(t14);
Texture2D NoiseTexture : register(t15);

// Sampler
SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);
SamplerState anisotropicSampler : register(s2);

static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

float4 GaussianBlur(float2 UV)
{
    float4 retValue = (float4) 0.0f;
    
    if (1.f < UV.x)
        UV.x = frac(UV.x);
    else if (UV.x < 0.0f)
        UV.x = 1.0f + frac(UV.x);
        
    if (1.f < UV.y)
        UV.y = frac(UV.y);
    else if (UV.y < 0.0f)
        UV.y = 1.0f + frac(UV.y);
    
    int2 iUV = UV * cNoiseTextureSize.xy;
    iUV -= int2(2, 2);
    
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            int2 idx = int2(iUV.y + i, iUV.x + j);
            retValue += NoiseTexture[idx] * GaussianFilter[i][j];
        }
    }
    
    return retValue;
}

void CalculateLight2D(in out float4 lightColor, float3 position, int idx)
{
    // Directional
    if (0 == lightsAttribute[idx].LightType)
    {
        lightColor += lightsAttribute[idx].Color;
    }
    // Point
    else if (1 == lightsAttribute[idx].LightType)
    {
        float length = distance(position.xy, lightsAttribute[idx].Position.xy);
        
        if (length < lightsAttribute[idx].LightRadius)
        {
            float ratio = 1.0f - (length / lightsAttribute[idx].LightRadius);
            lightColor += lightsAttribute[idx].Color * ratio;
        }
    }
    // Spot
    else
    {
        
    }
}
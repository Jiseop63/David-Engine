#include "Constant Buffer.hlsli"
#include "Structured Buffer.hlsli"
// Texture
Texture2D BindingTexture : register(t0);
Texture2D AtlasTexture : register(t12);

StructuredBuffer<LightAttribute> lightsAttribute : register(t13);

// Sampler
SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);
SamplerState anisotropicSampler : register(s2);

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
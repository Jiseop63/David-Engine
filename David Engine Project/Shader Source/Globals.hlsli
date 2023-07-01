#include "Constant Buffer.hlsli"
#include "Structures.hlsli"


Texture2D BindingTexture : register(t0);

SamplerState pointSampler : register(s0);
SamplerState anisotropicSampler : register(s1);
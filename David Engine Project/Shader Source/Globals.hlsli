#include "Constant Buffer.hlsli"

// Texture
Texture2D BindingTexture : register(t0);

// Sampler
SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);
SamplerState anisotropicSampler : register(s2);

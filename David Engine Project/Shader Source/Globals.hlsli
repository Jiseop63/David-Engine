#include "Constant Buffer.hlsli"

// Texture
Texture2D BindingTexture : register(t0);

// Sampler
SamplerState pointSampler : register(s0);
SamplerState anisotropicSampler : register(s1);

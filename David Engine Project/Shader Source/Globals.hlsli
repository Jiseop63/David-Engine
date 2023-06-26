

cbuffer Transform : register(b0)
{
    row_major matrix mWorld;
    row_major matrix mView;
    row_major matrix mProjection;
}


Texture2D BindingTexture : register(t0);

SamplerState pointSampler : register(s0);
SamplerState anisotropicSampler : register(s1);
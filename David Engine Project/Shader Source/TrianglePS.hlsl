struct VSIn
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

Texture2D sampleTexture : register(t0);
SamplerState samplerState : register(s0);

float4 main(VSOut In) : SV_Target
{
    float4 color = (float)0.0f;
    color = sampleTexture.Sample(samplerState, In.UV);
    
    return color;
}
#include "Globals.hlsli"

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


float4 mainPS(VSOut In) : SV_TARGET
{
    float4 color = float(0.0f);
    
    // default magenta : none collision creature
    if (0 == cColliderColorType)
    {
        color = float4(1.0f, 0.0f, 1.0f, 1.0f);        
    }
    // green : creature
    if (1 == cColliderColorType)
    {
        color = float4(0.0f, 1.0f, 0.0f, 1.0f);        
    }
    // red : collision creature
    if (2 == cColliderColorType)
    {
        color = float4(1.0f, 0.0f, 0.0f, 1.0f);        
    }
    return color;
}
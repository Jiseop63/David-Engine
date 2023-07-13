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
    
    // 조건에 따라 색을 바꿀 예정
    color = float4(0.0f, 1.0f, 0.0f, 1.0f);
    
    return color;
}
#include "globals.hlsli"

struct VSIn
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};
struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
    float2 GridPos : POSITION;
};

VSOut mainVS(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    float2 gridPos = float2(0.0f, 0.0f);
    
    float moveCoefficient = 100.0f;
    
    gridPos.x = In.Pos.x * 1.0f * cResolution.x + cCameraPosition.x * cCameraScale.x * moveCoefficient;
    gridPos.y = In.Pos.y * 1.0f * cResolution.y + cCameraPosition.y * cCameraScale.y * moveCoefficient;
    
    // ±‚¡ÿ¡°
    const float meshScale = 2.0f;
    
    Out.Pos = float4(In.Pos.xy * meshScale, 0.999f, 1.0f);
    Out.UV = In.UV;
    Out.GridPos = gridPos;
    
    return Out;
}

float4 mainPS(VSOut Out) : SV_Target
{
    float4 color = (float) 0.0f;
    
    return color;
}
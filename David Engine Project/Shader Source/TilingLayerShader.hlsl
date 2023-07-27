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
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

VSOut mainVS(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    float4 world = mul(float4(In.Pos, 1.0f), cWorld);
    float4 view = mul(world, cView);
    float4 projection = mul(view, cProjection);
    
    float2 newUV = float2(0.0f, 0.0f);
    
    // 추후에 CB로 받아서 속도 조절하면 될듯
    float moveCoefficient = 0.10f;
    
    
    
    newUV.x = In.UV.x + cCameraPosition.x * cCameraScale.x * moveCoefficient;
    // y값은 문제가 좀 있음
    newUV.y = In.UV.y;
    
    Out.Pos = projection;
    Out.WorldPos = world.xyz;
    Out.Color = In.Color;
    Out.UV = newUV;
    
    return Out;
}



float4 mainPS(VSOut Out) : SV_Target
{
    float4 color = (float) 0.0f;
        
    color = BindingTexture.Sample(pointSampler, Out.UV);
    
    // 기본색상 (어두움)
    float4 lightColor = float4(0.2f, 0.2f, 0.2f, 1.0f);
    
    for (int i = 0; i < 2; i++)
    {
        CalculateLight2D(lightColor, Out.WorldPos, i);
    }
    
    color *= lightColor;
    
    return color;
}
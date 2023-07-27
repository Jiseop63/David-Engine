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
    
    Out.Pos = projection;
    Out.WorldPos = world.xyz;
    Out.Color = In.Color;
            
    float2 offset = float2(0.01f + cValue, 0.0f); // 이동할 양
    offset *= cElapsedTime / 2.0f;
    float2 newUV = In.UV + offset;
    
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
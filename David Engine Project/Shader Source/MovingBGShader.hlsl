#include "Globals.hlsli"

VSOut mainVS(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    float4 world = mul(float4(In.Pos, 1.0f), cWorld);   
    float4 view = mul(world, cView);
    float4 projection = mul(view, cProjection);         
    
    Out.Pos = projection;
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
    
    return color;
}
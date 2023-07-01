#include "Globals.hlsli"

VSOut mainVS(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    //float time = ...;                 // 시간 값  <- cbuffer에서 받을 값
    float2 offset = float2(0.0f, 0.2);  // 이동할 양
    //float2 newUV = In.UV + offset * time;
    
    float4 world = mul(float4(In.Pos, 1.0f), mWorld);   
    float4 view = mul(world, mView);
    float4 projection = mul(view, mProjection);         
    
    Out.Pos = projection;
    Out.Color = In.Color;
    Out.UV = In.UV;
    //Out.UV = newUV;                               <- 변경할 UV 값
    
    return Out;
}

float4 mainPS(VSOut In) : SV_Target
{
    float4 color = (float) 0.0f;
    color = BindingTexture.Sample(pointSampler, In.UV);
    
    return color;
}
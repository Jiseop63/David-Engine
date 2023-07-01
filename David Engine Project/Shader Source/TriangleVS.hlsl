#include "Globals.hlsli"

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    float4 world = mul(float4(In.Pos, 1.0f), mWorld);
    float4 view = mul(world, mView);
    float4 projection = mul(view, mProjection);
    
    Out.Pos = projection;
    Out.Color = In.Color;
    Out.UV = In.UV;
    
    return Out;
}
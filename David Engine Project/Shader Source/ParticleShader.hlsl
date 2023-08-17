#include "Globals.hlsli"

struct VSIn
{
    float3 pos : POSITION;
    uint Instance : SV_InstanceID;
};

struct VSOut
{
    float4 pos : SV_Position;
};

VSOut mainVS(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
   
    float4 worldPos = mul(float4(In.pos, 1.0f), cWorld);
    worldPos.xyz += particles[In.Instance].Position.xyz;
    
    float4 viewPos = mul(worldPos, cView);
    Out.pos = mul(viewPos, cProjection);
    
    return Out;
}

float4 mainPS(VSOut In) : SV_TARGET
{
    float4 Out = (float4) 0.0f;
    
    Out = float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    return Out;

}
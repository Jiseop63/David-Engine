#include "Globals.hlsli"

struct VSIn
{
    float3 pos : POSITION;
    uint Instance : SV_InstanceID;
};

struct VSOut
{
    float4 pos : SV_POSITION;
};


VSOut mainVS(VSIn In)
{
    VSOut retValue = (VSOut) 0.0f;
    
    float4 worldPos = mul(float4(In.pos, 1.0f), cWorld);
    worldPos.xyz += particles[In.Instance].ParticlePosition.xyz;
    
    float4 viewPos = mul(worldPos, cView);
    retValue.pos = mul(viewPos, cProjection);
    
    return retValue;
}

float4 mainPS(VSOut In) : SV_TARGET
{
    float4 retValue = (float4) 0.0f;
    
    retValue = float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    return retValue;

}
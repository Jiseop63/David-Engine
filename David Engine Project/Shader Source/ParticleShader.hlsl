#include "Globals.hlsli"

struct VSIn
{
    float3 LocalPos : POSITION;
    uint Instance : SV_InstanceID;
};

struct VSOut
{
    float4 LocalPos : SV_Position;
    uint Instance : SV_InstanceID;
};

struct GSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};


VSOut mainVS(VSIn In)
{
    VSOut retValue = (VSOut) 0.0f;
    
    retValue.LocalPos = float4(In.LocalPos, 1.0f);
    retValue.Instance = In.Instance;
    
    return retValue;
}

[maxvertexcount(6)]
void mainGS(point VSOut In[1], inout TriangleStream<GSOut> output)
{
    GSOut Rect[4] = { (GSOut) 0.0f, (GSOut) 0.0f, (GSOut) 0.0f, (GSOut) 0.0f };
    
    if (particles[In[0].Instance].ParticleActive == 0)
        return;
    
    float3 worldPos = (In[0].LocalPos.xyz) + particles[In[0].Instance].ParticlePosition.xyz;
    
    float3 viewPos = mul(float4(worldPos, 1.0f), cView).xyz;
    
    float3 NewPos[4] =
    {
        viewPos - float3(-0.5f, 0.5f, 0.f) * float3(0.2f, 0.2f, 1.f),
        viewPos - float3(0.5f, 0.5f, 0.f) * float3(0.2f, 0.2f, 1.f),
        viewPos - float3(0.5f, -0.5f, 0.f) * float3(0.2f, 0.2f, 1.f),
        viewPos - float3(-0.5f, -0.5f, 0.f) * float3(0.2f, 0.2f, 1.f)
    };
    
    for (int i = 0; i < 4; ++i)
    {
        Rect[i].Pos = mul(float4(NewPos[i], 1.0f), cProjection);
    }
    
    Rect[0].UV = float2(0.0f, 0.0f);
    Rect[1].UV = float2(1.0f, 0.0f);
    Rect[2].UV = float2(1.0f, 1.0f);
    Rect[3].UV = float2(0.0f, 1.0f);
        
    // 0 -- 1
    // | \  |
    // 3 -- 2
    
    output.Append(Rect[0]);
    output.Append(Rect[1]);
    output.Append(Rect[2]);
    output.RestartStrip();

    output.Append(Rect[0]);
    output.Append(Rect[2]);
    output.Append(Rect[3]);
    output.RestartStrip();
}

float4 mainPS(GSOut In) : SV_TARGET
{
    float4 retValue = (float4) 0.0f;
    
    retValue = float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    return retValue;
}
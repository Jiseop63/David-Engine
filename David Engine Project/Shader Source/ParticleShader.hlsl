#include "Globals.hlsli"

struct VSIn
{
    float3 localPos : POSITION;
    uint Instance : SV_InstanceID;
};

struct VSOut
{
    float4 localPos : SV_Position;
    uint Instance : SV_InstanceID;
};

struct GSOut
{
    float4 pos : SV_Position;
    float2 UV : TEXCOORD;
};

VSOut mainVS(VSIn In)
{
    VSOut retValue = (VSOut) 0.0f;
    
    retValue.localPos = float4(In.localPos, 1.0f);
    retValue.Instance = In.Instance;
    
    return retValue;
}

[maxvertexcount(6)]
void mainGS(point VSOut In[1], inout TriangleStream<GSOut> output)
{
    // �̰� ����
    GSOut Out[4] = { (GSOut) 0.0f, (GSOut) 0.0f, (GSOut) 0.0f, (GSOut) 0.0f };
    
    // Active�� �ƴϸ� ����X
    if (particles[In[0].Instance].Active == 0)
        return;
    
    
    float3 worldPos = (In[0].localPos.xyz) + particles[In[0].Instance].Position.xyz;    
    float3 viewPos = mul(float4(worldPos, 1.0f), cView).xyz;
    
    // �̰� ����
    float3 NewPos[4] =
    {
        viewPos - float3(-0.5f, 0.5f, 0.f) * float3(0.2f, 0.2f, 1.f),
        viewPos - float3(0.5f, 0.5f, 0.f) * float3(0.2f, 0.2f, 1.f),
        viewPos - float3(0.5f, -0.5f, 0.f) * float3(0.2f, 0.2f, 1.f),
        viewPos - float3(-0.5f, -0.5f, 0.f) * float3(0.2f, 0.2f, 1.f)
    };
    
    // �Ƹ� �𼭸��ε�
    for (int edge = 0; edge < 4; ++edge)
    {
        Out[edge].pos = mul(float4(NewPos[edge], 1.0f), cProjection);
    }
    // �� �𼭸��� UV�� ����
    Out[0].UV = float2(0.0f, 0.0f);
    Out[1].UV = float2(1.0f, 0.0f);
    Out[2].UV = float2(1.0f, 1.0f);
    Out[3].UV = float2(0.0f, 1.0f);
    
    
    // 0 -- 1
    // | \  |
    // 3 -- 2
    
    // Triangle 2���� �̾������ ������
    output.Append(Out[0]);
    output.Append(Out[1]);
    output.Append(Out[2]);
    output.RestartStrip();

    output.Append(Out[0]);
    output.Append(Out[2]);
    output.Append(Out[3]);
    output.RestartStrip();
}

float4 mainPS(GSOut In) : SV_TARGET
{
    float4 retValue = (float4) 0.0f;
    
    retValue = float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    return retValue;

}
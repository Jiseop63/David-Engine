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
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

float2 RepeatUV(float2 uv, float scale)
{
    return float2(frac(uv.x * scale), uv.y);
}

VSOut mainVS(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    float4 world = mul(float4(In.Pos, 1.0f), cWorld);
    float4 view = mul(world, cView);
    float4 projection = mul(view, cProjection);
    
    float2 newUV = float2(0.0f, 0.0f);
    
    // ���Ŀ� CB�� �޾Ƽ� �ӵ� �����ϸ� �ɵ�
    float moveCoefficient = 0.10f;
    
    
    
    newUV.x = In.UV.x + cCameraPosition.x * cCameraScale.x * moveCoefficient;
    // y���� ������ �� ����
    newUV.y = In.UV.y;
    
    Out.Pos = projection;
    Out.Color = In.Color;
    Out.UV = newUV;
    
    return Out;
}



float4 mainPS(VSOut Out) : SV_Target
{
    float4 color = (float) 0.0f;
        
    color = BindingTexture.Sample(pointSampler, Out.UV);
    return color;
}
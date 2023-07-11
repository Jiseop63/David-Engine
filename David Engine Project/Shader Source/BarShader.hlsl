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

VSOut mainVS(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    float4 world = mul(float4(In.Pos, 1.0f), cWorld);
    float4 view = mul(world, cView);
    float4 projection = mul(view, cProjection);
    
    Out.Pos = projection;
    Out.Color = In.Color;
    //float ratio = 1.0f;
    //float2 newUV;
    //if (0 != cMaxValue)
    //{
    //    ratio = cCurrentValue / cMaxValue;
    //}
     
    
    //newUV.x = In.UV.x * (1 - ratio);
    //newUV.y = In.UV.y;
    //Out.UV = newUV;
    Out.UV = In.UV;
    return Out;
}

float4 mainPS(VSOut Out) : SV_Target
{
    float4 color = (float) 0.0f;
    //float4 color = float4(0.920f, 0.280f, 0.280f, 1.0f);

    float ratio = cCurrentValue / cMaxValue;
    if (Out.UV.x > ratio)
    {
        return float4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    color = BindingTexture.Sample(pointSampler, Out.UV);

    return color;
}
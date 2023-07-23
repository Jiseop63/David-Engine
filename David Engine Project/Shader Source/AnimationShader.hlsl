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
    
    if (cReverse)
    {
        Out.UV.x = 1.0f - In.UV.x;
        Out.UV.y = In.UV.y;
    }
    else
    {
        Out.UV = In.UV;
    }
    
    return Out;
}

float4 mainPS(VSOut Out) : SV_Target
{
    float4 color = (float) 0.0f;
    
    float2 diff = (cAtlasSize - cSpriteSize) / 2.0f;
    float2 UV = (cSpriteLeftTop - diff - cSpriteOffset)
                + (cAtlasSize * Out.UV);
    
    if (UV.x < cSpriteLeftTop.x || UV.x > cSpriteLeftTop.x + cSpriteSize.x
        || UV.y < cSpriteLeftTop.y || UV.y > cSpriteLeftTop.y + cSpriteSize.y)
        discard;
        
    color = AtlasTexture.Sample(pointSampler, UV);
    return color;
}
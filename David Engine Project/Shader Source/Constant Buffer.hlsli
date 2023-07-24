cbuffer cTransform : register(b0)
{
    row_major matrix cWorld;
    row_major matrix cView;
    row_major matrix cProjection;
}

cbuffer cTime : register(b2)
{
    float cElapsedTime;
    float cValue;
}

cbuffer cGrid : register(b3)
{
    float4 cCameraPosition;
    float2 cCameraScale;
    float2 cResolution;
}

cbuffer cLife : register(b4)
{
    float cMaxHP;
    float cCurHP;
}
cbuffer cCount : register(b5)
{
    float cMaxCount;
    float cCurCount;
}

cbuffer Animation : register(b6)
{
    float2 cSpriteLeftTop;
    float2 cSpriteSize;
    float2 cSpriteOffset;
    float2 cAtlasSize;
}

cbuffer Reverse : register(b7)
{
    bool cSideReverse;
    bool cVerticalReverse;
}
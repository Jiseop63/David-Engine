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

cbuffer cBar : register(b4)
{
    float cMaxValue;
    float cCurrentValue;
}
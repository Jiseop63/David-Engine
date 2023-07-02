cbuffer cTransform : register(b0)
{
    row_major matrix cWorld;
    row_major matrix cView;
    row_major matrix cProjection;
}

cbuffer cTime : register(b1)
{
    float cAddTime;
}
struct LightAttribute
{
    float4  Color;
    float4  Position;
    float4  Direction;
    
    uint    LightType;
    float   LightRadius;
    float   LightAngle;
    int     Padding;
};
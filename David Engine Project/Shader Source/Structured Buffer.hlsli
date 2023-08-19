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

struct Particle
{
    float4  Position;
    float4  Direction;
    
    float   EndTime;
    float   Time;
    float   Speed;
    uint    Active;
};

StructuredBuffer<LightAttribute> lightsAttribute : register(t13);
StructuredBuffer<Particle> particles : register(t14);

void CalculateLight2D(in out float4 lightColor, float3 position, int idx)
{
    // Directional
    if (0 == lightsAttribute[idx].LightType)
    {
        lightColor += lightsAttribute[idx].Color;
    }
    // Point
    else if (1 == lightsAttribute[idx].LightType)
    {
        float length = distance(position.xy, lightsAttribute[idx].Position.xy);
        
        if (length < lightsAttribute[idx].LightRadius)
        {
            float ratio = 1.0f - (length / lightsAttribute[idx].LightRadius);
            lightColor += lightsAttribute[idx].Color * ratio;
        }
    }
    // Spot
    else
    {
        
    }
}
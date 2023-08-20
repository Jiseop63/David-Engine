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
    float   MaxSpeed;
    float   Speed;
    uint    Active;
};

struct ParticleShared
{
    uint ActiveSharedCount;
};

StructuredBuffer<LightAttribute> lightsAttribute : register(t13);
StructuredBuffer<Particle> particles : register(t14);


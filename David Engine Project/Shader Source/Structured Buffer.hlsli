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
    float4  ParticleDirection;
    float4  ParticlePosition;
    
    float   ParticleLifeTime;
    float   ParticleAccumulatetime;
    float   ParticleStartSpeed;
    float   ParticleCurrentSpeed;
    uint    ParticleActive;
};

StructuredBuffer<LightAttribute> lightsAttribute : register(t13);
StructuredBuffer<Particle> particles : register(t14);
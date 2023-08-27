#include "Globals.hlsli"

struct VSIn
{
    float3 localPos : POSITION;
    uint Instance : SV_InstanceID;
};

struct VSOut
{
    float4 localPos : SV_Position;
    uint Instance : SV_InstanceID;
};

struct GSOut
{
    float4 pos : SV_Position;
    float2 UV : TEXCOORD;
};

VSOut mainVS(VSIn In)
{
    VSOut retValue = (VSOut) 0.0f;
    
    retValue.localPos = float4(In.localPos, 1.0f);
    retValue.Instance = In.Instance;
    
    return retValue;
}

[maxvertexcount(6)]
void mainGS(point VSOut In[1], inout TriangleStream<GSOut> output)
{
    // �̰� ����
    GSOut Out[4] = { (GSOut) 0.0f, (GSOut) 0.0f, (GSOut) 0.0f, (GSOut) 0.0f };
    
    // Active ���°� �ƴ϶�� ����X
    if (particles[In[0].Instance].Active == 0)
        return;
    
    
    float3 worldPos = (In[0].localPos.xyz) + particles[In[0].Instance].Position.xyz;    
    float3 viewPos = mul(float4(worldPos, 1.0f), cView).xyz;
    
    // �Ƹ� ������ġ�� ����?�����ε�
    float3 NewPos[4] =
    {
        viewPos - float3(-0.5f, 0.5f, 0.f) * float3(0.2f, 0.2f, 1.f),
        viewPos - float3(0.5f, 0.5f, 0.f) * float3(0.2f, 0.2f, 1.f),
        viewPos - float3(0.5f, -0.5f, 0.f) * float3(0.2f, 0.2f, 1.f),
        viewPos - float3(-0.5f, -0.5f, 0.f) * float3(0.2f, 0.2f, 1.f)
    };
    
    // �Ƹ� �𼭸� ��ġ��
    for (int edge = 0; edge < 4; ++edge)
    {
        Out[edge].pos = mul(float4(NewPos[edge], 1.0f), cProjection);
    }
    // �� �𼭸��� UV�� ����
    Out[0].UV = float2(0.0f, 0.0f);
    Out[1].UV = float2(1.0f, 0.0f);
    Out[2].UV = float2(1.0f, 1.0f);
    Out[3].UV = float2(0.0f, 1.0f);
    
    
    // 0 -- 1
    // | \  |
    // 3 -- 2
    
    // Triangle 2���� �̾������ ������
    output.Append(Out[0]);
    output.Append(Out[1]);
    output.Append(Out[2]);
    output.RestartStrip();

    output.Append(Out[0]);
    output.Append(Out[2]);
    output.Append(Out[3]);
    output.RestartStrip();
}

float4 mainPS(GSOut In) : SV_TARGET
{
    float4 retValue = (float4) 0.0f;
    retValue = BindingTexture.Sample(pointSampler, In.UV);
    
    if (retValue.a <= 0.0f)
        retValue = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    return retValue;

}

RWStructuredBuffer<Particle> ParticleBuffer : register(u0);
RWStructuredBuffer<ParticleShared> ParticleSharedBuffer : register(u1);
[numthreads(128, 1, 1)]
void mainCS(uint3 DTid : SV_DispatchThreadID)
{
    // ?
    if (DTid.x >= cParticleElementCount)
        return;
    
    // Active�� 0�̸� ���� ���°�����
    if (0 == ParticleBuffer[DTid.x].Active)
    {
        // SharedBuffer���� ActiveSharedCount�� 0�� �Ǹ� ����
        while (0 < ParticleSharedBuffer[0].ActiveSharedCount)
        {
            // origin���� �����ϰ� -1 �������� exchange�� ����
            int origin = ParticleSharedBuffer[0].ActiveSharedCount;
            int exchange = origin - 1;
            
            //InterlockedExchange(ParticleSharedBuffer[0].ActiveSharedCount, exchange, exchange);
            // ������ ����ȭ�� ���� �񱳰��� ��ġ�ϴ��� Ȯ���Ͽ� ��ġ�ϸ� �Է°����� �����
            // ParticleSharedBuffer[0].ActiveSharedCount �̰��� origin �̰Ͱ� ���Ͽ� ���� ��ġ�ϸ� exchange ���� �����
            InterlockedCompareExchange(ParticleSharedBuffer[0].ActiveSharedCount, origin, exchange, exchange);
            
            // �����忡 ���ؼ� ����ȭ�� ����� ��� Active ���·� �����ϱ�
            if (exchange == origin)
            {
                ParticleBuffer[DTid.x].Active = 1;
                break;
            }
        }
        // ���� ��ƼŬ�� �������¶��
        if (1 == ParticleBuffer[DTid.x].Active)
        {
            // ���������� ��ġ�� ������ �����ؾ��Ѵ�
            // ���ø��� �õ��� UV �� ����ؾ��Ѵ�
            float4 vRandom = (float4) 0.0f;
            // UV���� ... vector2 ( ���� ������ / �ִ� ��ƼŬ ����, ���� )
            float2 vUV = float2((float) DTid.x / cParticleElementCount, 0.50f);
            // UV �� x���� y���� �����ϰ� ����?�Ѵ�
            vUV.x += cElapsedTime;
            vUV.y += sin((vUV.x + cParticleElapsedTime) * 3.141592f + 2.0f * 10.0f) * 0.5f;
            
            // ���������� �������� UV������ ����þ� ���Ͱ��� �־ ���Ѵ�
            vRandom = float4
            (
                GaussianBlur(vUV + float2(0.0f, 0.0f)).x
                , GaussianBlur(vUV + float2(0.10f, 0.0f)).x
                , GaussianBlur(vUV + float2(0.20f, 0.0f)).x
                , GaussianBlur(vUV + float2(0.30f, 0.0f)).x
            );
            // ��ƼŬ�� ��ġ�� ���������� ���ϰ�, offset���� x y z�� ���ݽ� �̼������Ѵ�
            //ParticleBuffer[DTid.x].Position.xyz = vRandom.xyz * 3.0f;
            //ParticleBuffer[DTid.x].Position.x -= 0.65f;
            //ParticleBuffer[DTid.x].Position.y -= 1.4f;
            //ParticleBuffer[DTid.x].Position.z = 0.0f;
            
            ParticleBuffer[DTid.x].Position.x = vRandom.x * 2.50f;
            ParticleBuffer[DTid.x].Position.x -= 0.770f;
            ParticleBuffer[DTid.x].Position.y = -0.40f;
            //ParticleBuffer[DTid.x].Position.y -= 1.4f;
            //ParticleBuffer[DTid.x].Position.z = 0.0f;
            ParticleBuffer[DTid.x].Speed = ParticleBuffer[DTid.x].MaxSpeed;
            
                      
        }
    }
    // �̹� ���õ� ��ƼŬ�̹Ƿ� ���� �ൿ ��ħ�� �Ʒ��� ���Ѵ�
    else
    {
        // Ȱ��ȭ ���¸� �̵�
        if (1 == ParticleBuffer[DTid.x].Active)
        {
            ParticleBuffer[DTid.x].Speed -= cParticleDeltaTime / 1.0f;
            ParticleBuffer[DTid.x].Position += ParticleBuffer[DTid.x].Direction * ParticleBuffer[DTid.x].Speed * cParticleDeltaTime;
            ParticleBuffer[DTid.x].Time += cParticleDeltaTime;
            if (ParticleBuffer[DTid.x].EndTime <= ParticleBuffer[DTid.x].Time)
            {
                float4 vRandom = (float4) 0.0f;
                float2 vUV = float2((float) DTid.x / cParticleElementCount, 0.50f);
                vUV.x += cElapsedTime;
                vUV.y += sin((vUV.x + cParticleElapsedTime) * 3.141592f + 2.0f * 10.0f) * 0.5f;
                vRandom = float4
                (
                    GaussianBlur(vUV + float2(0.0f, 0.0f)).x
                    , GaussianBlur(vUV + float2(0.10f, 0.0f)).x
                    , GaussianBlur(vUV + float2(0.20f, 0.0f)).x
                    , GaussianBlur(vUV + float2(0.30f, 0.0f)).x
                );
                ParticleBuffer[DTid.x].Position.x = vRandom.x * 3.0f;
                ParticleBuffer[DTid.x].Position.x -= 0.870f;
                ParticleBuffer[DTid.x].Position.y = -0.40f;
                ParticleBuffer[DTid.x].Time = 0.0f;
                ParticleBuffer[DTid.x].Speed = ParticleBuffer[DTid.x].MaxSpeed;
                ParticleBuffer[DTid.x].Active = 0;

            }
        }        
    }    
}
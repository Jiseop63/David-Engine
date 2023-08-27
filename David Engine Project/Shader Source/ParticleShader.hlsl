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
    // 이게 뭐지
    GSOut Out[4] = { (GSOut) 0.0f, (GSOut) 0.0f, (GSOut) 0.0f, (GSOut) 0.0f };
    
    // Active 상태가 아니라면 생성X
    if (particles[In[0].Instance].Active == 0)
        return;
    
    
    float3 worldPos = (In[0].localPos.xyz) + particles[In[0].Instance].Position.xyz;    
    float3 viewPos = mul(float4(worldPos, 1.0f), cView).xyz;
    
    // 아마 월드위치의 비율?적용인듯
    float3 NewPos[4] =
    {
        viewPos - float3(-0.5f, 0.5f, 0.f) * float3(0.2f, 0.2f, 1.f),
        viewPos - float3(0.5f, 0.5f, 0.f) * float3(0.2f, 0.2f, 1.f),
        viewPos - float3(0.5f, -0.5f, 0.f) * float3(0.2f, 0.2f, 1.f),
        viewPos - float3(-0.5f, -0.5f, 0.f) * float3(0.2f, 0.2f, 1.f)
    };
    
    // 아마 모서리 위치값
    for (int edge = 0; edge < 4; ++edge)
    {
        Out[edge].pos = mul(float4(NewPos[edge], 1.0f), cProjection);
    }
    // 각 모서리의 UV값 세팅
    Out[0].UV = float2(0.0f, 0.0f);
    Out[1].UV = float2(1.0f, 0.0f);
    Out[2].UV = float2(1.0f, 1.0f);
    Out[3].UV = float2(0.0f, 1.0f);
    
    
    // 0 -- 1
    // | \  |
    // 3 -- 2
    
    // Triangle 2개를 이어붙혀서 렌더링
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
    
    // Active가 0이면 꺼진 상태같은데
    if (0 == ParticleBuffer[DTid.x].Active)
    {
        // SharedBuffer에서 ActiveSharedCount가 0이 되면 종료
        while (0 < ParticleSharedBuffer[0].ActiveSharedCount)
        {
            // origin값을 저장하고 -1 낮은값을 exchange로 저장
            int origin = ParticleSharedBuffer[0].ActiveSharedCount;
            int exchange = origin - 1;
            
            //InterlockedExchange(ParticleSharedBuffer[0].ActiveSharedCount, exchange, exchange);
            // 쓰레드 동기화로 대상과 비교값이 일치하는지 확인하여 일치하면 입력값으로 덮어씌움
            // ParticleSharedBuffer[0].ActiveSharedCount 이값을 origin 이것과 비교하여 둘이 일치하면 exchange 값을 덮어씌움
            InterlockedCompareExchange(ParticleSharedBuffer[0].ActiveSharedCount, origin, exchange, exchange);
            
            // 쓰레드에 의해서 동기화가 적용된 경우 Active 상태로 변경하기
            if (exchange == origin)
            {
                ParticleBuffer[DTid.x].Active = 1;
                break;
            }
        }
        // 만약 파티클이 켜진상태라면
        if (1 == ParticleBuffer[DTid.x].Active)
        {
            // 랜덤값으로 위치와 방향을 설정해야한다
            // 샘플링을 시도할 UV 를 계산해야한다
            float4 vRandom = (float4) 0.0f;
            // UV값은 ... vector2 ( 현재 쓰레드 / 최대 파티클 개수, 절반 )
            float2 vUV = float2((float) DTid.x / cParticleElementCount, 0.50f);
            // UV 의 x값과 y값을 랜덤하게 구현?한다
            vUV.x += cElapsedTime;
            vUV.y += sin((vUV.x + cParticleElapsedTime) * 3.141592f + 2.0f * 10.0f) * 0.5f;
            
            // 최종적으로 랜덤값을 UV값에서 가우시안 필터값에 넣어서 구한다
            vRandom = float4
            (
                GaussianBlur(vUV + float2(0.0f, 0.0f)).x
                , GaussianBlur(vUV + float2(0.10f, 0.0f)).x
                , GaussianBlur(vUV + float2(0.20f, 0.0f)).x
                , GaussianBlur(vUV + float2(0.30f, 0.0f)).x
            );
            // 파티클의 위치를 랜덤값에서 구하고, offset으로 x y z를 조금식 미세조정한다
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
    // 이미 세팅된 파티클이므로 이후 행동 방침을 아래서 정한다
    else
    {
        // 활성화 상태면 이동
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
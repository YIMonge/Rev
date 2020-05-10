//********************************************************
struct VSIn
{
    float4 position : POSITION;
    float4 uv : TEXCOORD;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

PSInput main(VSIn In)
//PSInput main(float4 position : POSITION, float2 uv : TEXCOORD)
{
    PSInput result;

    result.position = In.position;
    result.uv = In.uv;

    return result;
}

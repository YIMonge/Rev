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

cbuffer ConstantBuffer : register(b0)
{
    float4 offset;
};

PSInput main(VSIn In)
{
    PSInput result;

    result.position = In.position + offset;
    result.uv = In.uv;// +offset2;

    return result;
}

//********************************************************
struct VSIn
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct PSInput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
};

PSInput main(const VSIn input)
{
    PSInput result = (PSInput)0;

    result.position = input.position;
    result.uv = input.uv;

    return result;
}

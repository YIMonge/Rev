struct VSIn
{
    float4 position : POSITION;
	float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct PSInput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
};

cbuffer global : register(b0)
{
	matrix view;
	matrix projection;
	matrix world;
	matrix wvp;
}


PSInput main(const VSIn input)
{
    PSInput result = (PSInput)0;

    result.position = input.position;
    result.uv = input.uv;

    return result;
}

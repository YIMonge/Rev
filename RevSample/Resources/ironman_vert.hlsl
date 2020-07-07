struct VSIn
{
    float4 position : POSITION;
	float3 normal : NORMAL;
};

struct PSInput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
};

cbuffer global : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	matrix wvp;
}


PSInput main(const VSIn input)
{
    PSInput result = (PSInput)0;
    result.position = mul(float4(input.position.xyz, 1), wvp);
    return result;
}

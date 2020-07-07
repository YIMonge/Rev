struct PSInput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(const PSInput input) : SV_Target
{
	return g_texture.Sample(g_sampler, float2(0.0, 0.0));
}

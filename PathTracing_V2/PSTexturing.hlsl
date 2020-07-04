

SamplerState textureSampler : Sampler : register(s0);
Texture2D someTexture : SOMETEXTURE : register(t0);

struct Input
{
	float4 pos : SV_POSITION;
	float2 textureCoord : TEXCOORD;
};

float4 main(Input input) : SV_TARGET
{
	return someTexture.Sample(textureSampler, input.textureCoord);
//return float4(0, 1, 0, 1);
}
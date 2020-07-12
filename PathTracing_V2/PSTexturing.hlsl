

SamplerState textureSampler : Sampler : register(s0);
Texture2D someTexture : SOMETEXTURE : register(t0);
cbuffer Cbuffer : register(b0)
{
	float3 testCameraPos : TESTCAMERAPOS;
	float2 testCameraAngle : TESTCAMERAANGLE;
}
struct Input
{
	float4 pos : SV_POSITION;
	float2 textureCoord : TEXCOORD;
};

float4 main(Input input) : SV_TARGET
{
	return someTexture.Sample(textureSampler, input.textureCoord);
	//return float4(testCameraPos, 1);
}
struct Input
{
	float4 pos : POSITION;
	float2 textureCoord : TEXCOORD;
};
struct Output
{
	float4 pos : SV_POSITION;
	float2 textureCoord : TEXCOORD;
};
Output main(Input input)
{
	Output output;
	output.pos = input.pos;
	output.textureCoord = input.textureCoord;
	return output;
}
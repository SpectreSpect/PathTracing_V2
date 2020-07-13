
cbuffer cBuffer
{
	float SamplesCount;
};
struct Ray
{
	float4 diraction;
	float4 origin;
	float t;
};
RWTexture2D<float4> Result : register(t0); // Вывод/ввод цвета
RWStructuredBuffer<Ray> ray : register(u0);


	[numthreads(8, 8, 1)]
void main(uint3 threadID : SV_DispatchThreadID)
{
	uint width, height;
	Result.GetDimensions(width, height);
	//float2 uv = float2(((threadID.x / width) * 2) - 1)

}
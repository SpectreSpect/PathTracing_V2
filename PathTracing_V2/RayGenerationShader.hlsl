
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
RWTexture2D<float4> Result : register(u0); // Вывод/ввод цвета
RWStructuredBuffer<Ray> ray : register(u1);


[numthreads(8, 8, 1)]
void main(uint3 threadID : SV_DispatchThreadID)
{
	uint width, height;
	Result.GetDimensions(width, height);
	float2 uv = float2(((threadID.xy / float2(width, height)) * 2) - 1);
	Result[threadID.xy] = float4(uv, 1, 1);
}
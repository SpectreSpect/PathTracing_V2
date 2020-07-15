
cbuffer cBuffer : register(b0)
{
	float samplesCount;
	float3 cameraPos;
	float2 cameraAngle;
};
struct Ray
{
	float t;
	float3 direction;
	float3 origin;
	float3 energy;
	float3 result;
};

RWTexture2D<float4> Result : register(u0);
RWStructuredBuffer<Ray> ray : register(u1);


Ray InitRay(float3 direction, float3 origin)
{
	Ray ray;
	ray.t = 0;
	ray.direction = direction;
	ray.origin = origin;
	ray.energy = 1;
	ray.result = 0;
	return ray;
}

float3 initPixelPos(float2 pixelPos, float2 cameraAngle, float viewingAngle, float screenResolutionCoefficient)
{
	float3 pos = float3(0, 0, 0);
	pos.x = (viewingAngle * sin(cameraAngle.x) * cos(cameraAngle.y)) + pixelPos.x * screenResolutionCoefficient * cos(cameraAngle.x) - pixelPos.y * sin(cameraAngle.y) * sin(cameraAngle.x);
	pos.y = viewingAngle * sin(cameraAngle.y) + pixelPos.y * cos(cameraAngle.y);
	pos.z = viewingAngle * cos(cameraAngle.x) * cos(cameraAngle.y) - pixelPos.x * screenResolutionCoefficient * sin(cameraAngle.x) - pixelPos.y * sin(cameraAngle.y) * cos(cameraAngle.x);
	return pos;
}

[numthreads(8, 8, 1)]
void main(uint3 threadID : SV_DispatchThreadID)
{
	uint width, height;
	Result.GetDimensions(width, height);
	float2 uv = float2(((threadID.xy / float2(width, height)) * 2) - 1);
	//float2 uv = float2((threadID.xy + float2(0.5f, 0.5f)) / float2(width, height) * 2.0f - 1.0f);
	uv = float2(uv.x, -uv.y);
	float3 diraction = initPixelPos(uv, cameraAngle, 1, 1);
	Result[threadID.xy] = float4((ray[threadID.x + threadID.y * width].result + Result[threadID.xy].xyz * (samplesCount - 1)) / samplesCount, Result[threadID.xy].w);
	ray[threadID.x + threadID.y * width] = InitRay(normalize(diraction), cameraPos);
}

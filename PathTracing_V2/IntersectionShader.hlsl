
struct SpherePrimetive
{
	float4 spherePos;
	//float radius;
	//int objID;
};
RWTexture2D<float4> Result : register(u0);
StructuredBuffer<SpherePrimetive> spherePrimetives : register(t0);

[numthreads(8, 8, 1)]
void main(uint3 threadID : SV_DispatchThreadID)
{
	float4 color = spherePrimetives[0].spherePos;
	Result[threadID.xy] = color;
}

struct SpherePrimetive
{
	float3 spherePos;
	float radius;
	int objID;
};
struct Ray
{
	float t;
	float3 direction;
	float3 origin;
	float3 energy;
	float3 result;
};
class Hit
{
	float t;
	float3 direction;
	float3 position;
	float3 normal;
	float3 albedo;
	float3 specular;
	float3 emission;
	float smooth;
	float4 eta;
	float2 uv;
};
Hit InitHit()
{
	Hit hit;
	hit.t = 1.#INF;
	hit.direction = 0;
	hit.position = 0;
	hit.normal = 0;
	hit.albedo = 0;
	hit.emission = 0;
	hit.specular = 0;
	hit.smooth = 0;
	hit.uv = 0;
	hit.eta = 0;
	return hit;
}
RWTexture2D<float4> Result : register(u0);
StructuredBuffer<SpherePrimetive> spherePrimetives : register(t0);
StructuredBuffer<Ray> ray : register(t1);

void IntersectSphere(Ray ray, inout Hit bestHit, float3 spherePos, float radius)
{
	float3 d = ray.origin - spherePos;
	float p1 = -dot(ray.direction, d);
	float p2sqr = p1 * p1 - dot(d, d) + radius * radius;
	if (p2sqr < 0)
		return;
	float p2 = sqrt(p2sqr);
	float t = p1 - p2 > 0 ? p1 - p2 : p1 + p2;
	if (t > 0 & t < bestHit.t)
	{
		bestHit.t = t;
		bestHit.position = (t * ray.direction) + ray.origin;
		bestHit.normal = normalize(bestHit.position - spherePos);
		bestHit.albedo = float3(0, 1, 1);
		return;
	}
}

[numthreads(8, 8, 1)]
void main(uint3 threadID : SV_DispatchThreadID)
{
	uint width, height;
	Result.GetDimensions(width, height);
	uint rayID = threadID.x + threadID.y * width;
	Hit hit = InitHit();
	IntersectSphere(ray[rayID], hit, spherePrimetives[0].spherePos, spherePrimetives[0].radius);
	Result[threadID.xy] = float4(hit.albedo / hit.t, 1);
}
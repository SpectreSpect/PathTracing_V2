
struct SpherePrimetive
{
	float3 position;
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
struct Hit
{
	float3 position;
	float t;
	float3 normal;
	int objID;
	float padding[3];
	int primitiveID;
};
Hit InitHit()
{
	Hit hit;
	hit.t = 1.#INF;
	hit.position = 0;
	hit.normal = 0;
	hit.objID = 0;
	hit.primitiveID = 0;
	for (int i = 0; i < 3; i++)
		hit.padding[i] = 0;
	return hit;
}
cbuffer PrimetivesCount
{
	int nTriangles;
	int nSpheres;
	int padding[2];
}
RWTexture2D<float4> Result : register(u0);
RWStructuredBuffer<Hit> hit : register(u1);
StructuredBuffer<SpherePrimetive> spherePrimetives : register(t0);
StructuredBuffer<Ray> ray : register(t1);

void IntersectSphere(Ray ray, inout Hit bestHit, SpherePrimetive sphere, int primitiveID)
{
	//bestHit.objID = -1;
	float3 d = ray.origin - sphere.position;
	float p1 = -dot(ray.direction, d);
	float p2sqr = p1 * p1 - dot(d, d) + sphere.radius * sphere.radius;
	if (p2sqr < 0)
		return;
	float p2 = sqrt(p2sqr);
	float t = p1 - p2 > 0 ? p1 - p2 : p1 + p2;
	if (t > 0 & t < bestHit.t)
	{
		bestHit.t = t;
		bestHit.position = (t * ray.direction) + ray.origin;
		bestHit.normal = normalize(bestHit.position - sphere.position);
		bestHit.objID = sphere.objID;
		bestHit.primitiveID = primitiveID;
		return;
	}
}

[numthreads(8, 8, 1)]
void main(uint3 threadID : SV_DispatchThreadID)
{
	uint width, height;
	Result.GetDimensions(width, height);
	uint rayID = threadID.x + threadID.y * width;
	hit[rayID] = InitHit();
	for (int i = 0; i < nSpheres; i++)
	{
		IntersectSphere(ray[rayID], hit[rayID], spherePrimetives[i], i);
	}
	//Result[threadID.xy] = float4(float3(0, 1, 1) / hit[rayID].t, 1);
}
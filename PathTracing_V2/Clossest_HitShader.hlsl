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
struct SpherePrimetive
{
	float3 position;
	float radius;
	int objID;
};
cbuffer objCBuffer : register(b0)
{
	float4 albedo;
	float4 emission;
	int objID;
	float _seed;
	float padding[2];
}
RWTexture2D<float4> Result : register(u0);
RWStructuredBuffer<Ray> ray : register(u1);
StructuredBuffer<Hit> hit : register(t0);

static const float PI = 3.14159265f;
static float2 _PixelPos;

float rand_1_05()
{
	float2 noise = (frac(sin(dot(_PixelPos.xy, float2(12.9898, 78.233) * 2.0 - _seed)) * 43758.5453 + _seed / 3));
	return abs(noise.x + noise.y) * 0.5;
}
float rand()
{
	float result = rand_1_05();
	return result;
}
float3x3 GetTangentSpace(float3 normal)
{
	float3 helper = float3(1, 0, 0);
	if (abs(normal.x) > 0.99f)
		helper = float3(0, 0, 1);

	float3 tangent = normalize(cross(normal, helper));
	float3 binormal = normalize(cross(normal, tangent));
	return float3x3(tangent, binormal, normal);
}

float3 SampleHemisphere(float3 normal, float alpha)
{
	float cosTheta = pow(1 - rand(), 1 / (alpha + 1.0f));
	float sinTheta = sqrt(1.0f - cosTheta * cosTheta);
	float phi = 2 * PI * rand();
	float3 tangentSpaceDir = float3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
	return mul(tangentSpaceDir, GetTangentSpace(normal));
}
float3 Albedo(inout Ray ray, Hit hit, float bias)
{
	ray.origin = hit.position + hit.normal * bias;
	ray.direction = SampleHemisphere(hit.normal, 1);
	return float3(1, 0, 0);
}

float3 Shade(inout Ray ray, Hit hit)
{
	float bias = 0.00015f;
	ray.energy *= Albedo(ray, hit, bias);
	return emission.xyz;	
}

[numthreads(8, 8, 1)]
void main(uint3 threadID : SV_DispatchThreadID)
{
	uint width, height;
	Result.GetDimensions(width, height);
	uint oneDID = threadID.x + threadID.y * width;
	float2 uv = float2(((threadID.xy / float2(width, height)) * 2) - 1);
	uv = float2(uv.x, -uv.y);
	_PixelPos = uv;
	if (hit[oneDID].objID == objID)
	{
		ray[oneDID].energy = ray[oneDID].energy * Shade(ray[oneDID], hit[oneDID]);
		Result[threadID.xy] = float4(albedo.xyz / hit[oneDID].t, 1);
	}

}
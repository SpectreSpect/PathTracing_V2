static float4 _PixelPos;
static int _Seed;
static float _Seed1;
static const float EPSILON = 1e-8;

struct Input
{
	float4 pixelPos : PIXELPOS;
	float4 pos : SV_POSITION;
	float2 textureCoord : TEXCOORD;
};

cbuffer CBuffer : register(b0)
{
	float4 vertexPos : VERTEXPOS;
	float4 sphere1Pos : SPHERE1POS;
	float4 k : KOF;
	float4 screenResolution : SCREENRESOLUTION;
	float4 cameraPos : CAMERAPOS;
	float4 cameraAngle : CAMERAANGLE;
	float4 randomValue : RANDOMVALUE;
	float4 random1Value : RANDOM1VALUE;
	float4 samplesCount : SAMPLESCOUNT;
	float4 velocity : VELOCITY;
	float4 circlePos : CIRCLEPOS;
};
struct MeshObj
{
	float indexOffset;
	float indecesCount;
};
struct Vertex
{
	float3 pos;
};
StructuredBuffer<MeshObj> _Meshes : register(t50);
StructuredBuffer<Vertex> _Vertices : register(t51);
StructuredBuffer<int> _Inices : register(t52);

cbuffer CModelBuffer : register(b1)
{
	//vector<float, 3> LOL;
	//MeshObj _MeshObj[1];
	//Vertex _Vertices[3];
	//int _Indeces[3];
}
static Texture2D NULLTEXTURE;
Texture2D objTexture : TEXTURE : register(t0);
Texture2D HDRtexture : HDRTEXTURE : register(t1);
Texture2D leatherTexture : LEATHERTEXTURE : register(t2);
Texture2D leatherNormal : LEATHERNORMAL : register(t3);

Texture2D textureLighted : TEXTURELIGHTED : register(t4);
Texture2D lightMap : LIGHTNORMAL : register(t5);
Texture2D tilesTexture : LIGHTNORMAL : register(t6);
Texture2D tilesNormalMap : LIGHTNORMAL : register(t7);
Texture2D tilesSpecularMap : LIGHTNORMAL : register(t8);

SamplerState objSamplerState : Sampler : register(s0);


static const float PI = 3.14159265f;
float sdot(float3 x, float3 y, float f = 1.0f)
{
	return saturate(dot(x, y) * f);
}

float Lerp(float val1, float val2, float k)
{
	return val1 + (val2 - val1) * k;
}
float Lerp2(float2 val1, float2 val2, float2 k)
{
	return val1 + (val2 - val1) * k;
}
float3 Lerp3(float3 val1, float3 val2, float3 k)
{
	return val1 + (val2 - val1) * k;
}
float4 Lerp4(float4 val1, float4 val2, double k)
{
	return val1 + (val2 - val1) * k;
}
float Rand(float randValue, float2 pixelPos, float seed, float iter)
{
	float next = (pixelPos * iter * randValue * iter * (sin(iter) + 2) * (cos(pixelPos) + 2) + iter * randValue * seed * iter + seed) + randValue + seed + pixelPos + iter;
	return ((next / 65536 % 1) * 2) - 1;
}
void Swap(inout float a, inout float b)
{
	float oldA = a;
	a = b;
	b = oldA;
}


	float getRand
	(
	int seed, int iter)
{
		int a = int((seed + iter) * 231 * _PixelPos.x * random1Value.x) % int((seed + iter) * 21 + random1Value.x * randomValue.x);
		int b = int((seed + iter) * 125 * _PixelPos.y) % int((seed + iter) * 34 + (random1Value.x % 163));
		return (int(a * int(randomValue.x + (seed + iter)) * b) % 1000) / 1000.0f;
	}

	float rand_1_05
	()
{
		float2 noise = (frac(sin(dot(_PixelPos.xy, float2(12.9898, 78.233) * 2.0 - _Seed)) * 43758.5453 + _Seed / 3));
		_Seed += _Seed1;
		return abs(noise.x + noise.y) * 0.5;
	}


	float2 rand_2_10
	()
{
		float noiseX = (frac(sin(dot(_PixelPos.xy, float2(12.9898, 78.233) * 2.0)) * 43758.5453)) * _Seed;
		_Seed += 5.0f;
		float noiseY = sqrt(1 - noiseX * noiseX) * _Seed;
		_Seed++;
		return frac(dot(noiseX, noiseY));
	}

	float rand
	()
{
		float result = rand_1_05();
	//float result = frac(sin((_Seed1 + _Seed * 34) / 100.0f * dot(_PixelPos.xy, float2(12.9898f, 78.233f))) * 43758.5453f);
	//_Seed += 1.0f;
	//_Seed1 += 5.0f;
		return result;
	}

class Ray
{
	float t;
	float3 direction;
	float3 origin;
	float3 energy;
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
class Sphere
{
	float3 position;
	float3 normal;
	float3 albedo;
	float3 emission;
	float3 specular;
	Texture2D text;
};
Ray InitRay(float3 direction, float3 origin)
{
	Ray ray;
	ray.t = 0;
	ray.direction = direction;
	ray.origin = origin;
	ray.energy = 1;
	return ray;
}
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
float3 maybeReflect(float3 v, float3 n)
{
	return v - (2 * dot(v, n) * length(v) * normalize(n));
}


float3x3 GetTangentSpace
(
	float3 normal)
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
	//float cosTheta = rand();
	//float cosTheta =  pow(rand(), 1.0f / (alpha + 1.0f));
	float cosTheta = pow(1 - rand(), 1 / (alpha + 1.0f));
	//float sinTheta = sqrt(max(0.0f, 1.0f - cosTheta * cosTheta));
	float sinTheta = sqrt(1.0f - cosTheta * cosTheta);
	float phi = 2 * PI * rand();
	float3 tangentSpaceDir = float3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
	return mul(tangentSpaceDir, GetTangentSpace(normal));
}
float3 testRefract(float3 l, float3 n, float k)
{
	return Lerp3(l, n, k);
}

void IntersectSphere
(Ray
	ray, inout
	Hit bestHit, float3 spherePos, float radius, float3 albedo, float3 emission, float3 specular = 0, float smooth = 0, float4 refractEta = 0)
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
		float3 normal = normalize(bestHit.position - spherePos);
		float inorout = dot(normal, ray.direction);
		if (inorout < 0)
		{
			bestHit.normal = normal;
		}
		else if (inorout > 0)
			bestHit.normal = -normal;
		bestHit.albedo = albedo;
		bestHit.emission = emission;
		bestHit.specular = specular;
		bestHit.smooth = smooth;
		bestHit.eta = refractEta;
		return;
	}
}
void IntersectSphereTextured
(Ray
	ray, inout
	Hit bestHit, float3 spherePos, float radius, float3 albedo, float3 emission, Texture2D text, float3 specular = 0, float smooth = 0)
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
		float theta = acos(bestHit.normal.y);
		float phi = atan2(bestHit.normal.x, bestHit.normal.z);
		bestHit.albedo = albedo * text.Sample(objSamplerState, float2(phi / (2 * PI), theta / PI)).xyz;
		bestHit.emission = emission;
		bestHit.specular = specular;
		bestHit.smooth = smooth;
		return;
	}
}
void IntersectSphereTextured
(Ray
	ray, inout
	Hit bestHit, float3 spherePos, float radius, float3 albedo, float3 emission, Texture2D text, Texture2D normalMap, float3 specular = 0, float smooth = 0)
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
		float3 normalToSphere = normalize(bestHit.position - spherePos);
		float theta = acos(normalToSphere.y);
		float phi = atan2(normalToSphere.x, normalToSphere.z);
		bestHit.normal = mul(normalMap.Sample(objSamplerState, float2(phi / (2 * PI), theta / PI)).xyz, GetTangentSpace(normalToSphere));
		bestHit.albedo = albedo * text.Sample(objSamplerState, float2(phi / (2 * PI), theta / PI)).xyz;
		bestHit.emission = emission;
		bestHit.specular = specular;
		bestHit.smooth = smooth;
		return;
	}
}
void IntersectSphereTextured(Ray ray, inout Hit bestHit, float3 spherePos, float radius, float3 albedo, float3 emission, Texture2D text, Texture2D normalMap, Texture2D specularTexture, float smooth = 0)
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


		float3 normalToSphere = normalize(bestHit.position - spherePos);
		float theta = acos(normalToSphere.y);
		float phi = atan2(normalToSphere.x, normalToSphere.z);
		bestHit.normal = mul(normalMap.Sample(objSamplerState, float2(phi / (2 * PI), theta / PI)).xyz, GetTangentSpace(normalToSphere));
		//bestHit.normal = mul(float3(1, 0, 0), GetTangentSpace(normalToSphere));
		bestHit.albedo = albedo * text.Sample(objSamplerState, float2(phi / (2 * PI), theta / PI)).xyz;
		bestHit.emission = emission;
		bestHit.specular = specularTexture.Sample(objSamplerState, float2(phi / (2 * PI), theta / PI)).xyz;
		bestHit.smooth = smooth;
		return;
	}
}
void IntersectSphereTexturedLight
(Ray

	ray, inout
	Hit bestHit, float3 spherePos, float radius, float3 albedo, float3 emission, Texture2D text, Texture2D lightMap, float3 specular = 0, float smooth = 0)
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

		float theta = acos(bestHit.normal.y);
		float phi = atan2(bestHit.normal.x, bestHit.normal.z);

		bestHit.albedo = albedo * text.Sample(objSamplerState, float2(phi / (2 * PI), theta / PI)).xyz;
		bestHit.emission = emission * lightMap.Sample(objSamplerState, float2(phi / (2 * PI), theta / PI)).xyz;
		bestHit.specular = specular;
		bestHit.smooth = smooth;
		return;
	}
}


void IntersectTriangleBase(Ray ray, float3 v0, float3 v1, float3 v2, inout float3 normal, inout float u, inout float v, inout float t)
{

	float3 v0v1 = v1 - v0;
	float3 v0v2 = v2 - v0;

	float3 p = cross(ray.direction, v0v2);
	float det = dot(v0v1, p);

	if (det == 0)
		return;
	float invdet = 1.0f / det;
	float3 tvec = ray.origin - v0;
	u = dot(tvec, p) * invdet;
	if (u < 0.0f || u > 1.0f)
		return;

	float3 q = cross(tvec, v0v1);
	v = dot(ray.direction, q) * invdet;
	if (v < 0.0f || u + v > 1.0f)
		return;
	t = dot(v0v2, q) * invdet;
	float3 normalTimely = normalize(cross(v0v2, v0v1));;
	float inorout = dot(normalTimely, ray.direction);
	if (inorout < 0)
	{
		normal = normalTimely;
	}
	else if (inorout > 0)
		normal = -normalTimely;
}

void IntersectTriangle
(Ray
	ray, inout
	Hit bestHit, float3 v0, float3 v1, float3 v2, float3 albedo, float3 emission, float3 specular = 0, float smooth = 0, float4 refractEta = 0)
{
	float3 normal = 0;
	float u = 0;
	float v = 0;
	float t = 0;
	IntersectTriangleBase(ray, v0, v1, v2, normal, u, v, t);

	if (t > 0 & t < bestHit.t)
	{
		bestHit.t = t;
		bestHit.position = ray.direction * t + ray.origin + normal * 0.0001f;
		bestHit.normal = normal;
		bestHit.albedo = albedo;
		bestHit.emission = emission;
		bestHit.specular = specular;
		bestHit.smooth = smooth;
		bestHit.uv = float2(u, v);
		bestHit.eta = refractEta;
	}
}

struct VertexTemp
{
	float3 coord;
	float2 uv;
};
void IntersectTriangle(Ray ray, inout Hit bestHit, Vertex v0, Vertex v1, Vertex v2)
{
	float3 normal = 0;
	float u = 0;
	float v = 0;
	float t = 0;
	IntersectTriangleBase(ray, v0.pos, v1.pos, v2.pos, normal, u, v, t);

	if (t > 0 & t < bestHit.t)
	{
		bestHit.t = t;
		bestHit.position = ray.direction * t + ray.origin + normal * 0.0001f;
		bestHit.normal = normal;
		//bestHit.albedo = v0.color * u + v1.color * v + v2.color * (1 - u - v);
		bestHit.albedo = 0;
		bestHit.emission = 0;
		bestHit.specular = 0;
		bestHit.smooth = 0;
		bestHit.uv = float2(u, v);
		bestHit.eta = float4(1, 1, 1, 1.5f);
	}
}

void IntersectMesh
	(Ray
	ray, inout
	Hit bestHit, int meshIndecs)
{
	for (int i = _Meshes[meshIndecs].indexOffset; i < _Meshes[meshIndecs].indecesCount; i += 3)
	{
		IntersectTriangle(ray, bestHit, _Vertices[_Inices[i]], _Vertices[_Inices[i + 1]], _Vertices[_Inices[i + 2]]);
	}
	//IntersectTriangle(ray, bestHit, _Vertices[_Inices[3]], _Vertices[_Inices[4]], _Vertices[_Inices[5]]);
	//Vertex fuck = _Vertices[_Inices[1]];
	//fuck.pos += float3(0, 0, 20);
	//IntersectTriangle(ray, bestHit, _Vertices[_Inices[0]], fuck, _Vertices[_Inices[2]]);
}

void IntersectTriangleTextured
	(Ray
	ray, inout
	Hit bestHit, Texture2D someTexture, VertexTemp
	v0, VertexTemp
	v1, VertexTemp
	v2,
	float3 albedo, float3 emission, float3 specular = 0, float smooth = 0)
{
	float3 normal = 0;
	float u = 0;
	float v = 0;
	float t = 0;
	IntersectTriangleBase(ray, v0.coord, v1.coord, v2.coord, normal, u, v, t);

	if (t > 0 & t < bestHit.t)
	{
		bestHit.t = t;
		bestHit.position = ray.direction * t + ray.origin + normal * 0.0001f;
		bestHit.normal = normal;
		bestHit.albedo = albedo * someTexture.SampleLevel(objSamplerState, v1.uv * u + v2.uv * v + v0.uv * (1 - u - v), 0);
		bestHit.emission = emission;
		bestHit.specular = specular;
		bestHit.smooth = smooth;
		bestHit.uv = float2(u, v);
	}
}
void IntersectFlat

	(Ray

	ray, inout
	Hit bestHit, float3 flatPos, float3 flatRotation, float3 albedo, float3 emission, float3 specular = 0, float smooth = 0)
{

	//float t = (flatPos.y - ray.origin.y) / ray.direction.y;
	//if (t > 0 && t < bestHit.t)
	//{
	//	bestHit.t = t;
	//	bestHit.position = ray.origin + t * ray.direction;
	//	bestHit.normal = float3(0.0f, 1.0f, 0.0f);
	//	bestHit.albedo = albedo;
	//	bestHit.emission = emission;
	//	bestHit.specular = specular;
	//	bestHit.smooth = smooth;
	//}
	flatRotation = normalize(flatRotation);
	float t = 0;
	//if (dot(ray.direction, flatRotation) != 0)
	//	t = dot(flatPos - ray.origin, flatRotation) / dot(ray.direction, flatRotation);
	if ((flatRotation.x * ray.direction.x + flatRotation.y * ray.direction.y + flatRotation.z * ray.direction.z) != 0)
		t = -(ray.origin.x * flatRotation.x + ray.origin.y * flatRotation.y + ray.origin.z * flatRotation.z - flatPos.x - flatPos.y - flatPos.z) / (flatRotation.x * ray.direction.x + flatRotation.y * ray.direction.y + flatRotation.z * ray.direction.z);
	if (t > 0 & t < bestHit.t)
	{
		bestHit.t = t;
		bestHit.position = ray.origin + t * ray.direction;
		bestHit.normal = flatRotation;
		bestHit.albedo = albedo;
		bestHit.emission = emission;
		bestHit.specular = specular;
		bestHit.smooth = smooth;
	}
	return;
}
float3 Colorizate

	(

	float3 color)
{
	return (color.x / 255, color.y / 255, color.z / 255);
}
float3 Colorizate

	(

	float colorX, float colorY, float colorZ)
{
	return float3((float) (colorX / 255), (float) (colorY / 255), (float) (colorZ / 255));
}

void IntersectQaud

	(Ray

	ray, inout
	Hit bestHit, float3 v0, float3 v1, float3 v2, float3 v3, float3 albedo, float3 emission, float3 specular = 0, float smooth = 0, float4 refractEta = 0)
{
	IntersectTriangle(ray, bestHit, v0, v1, v3, albedo, emission, specular, smooth, refractEta);
	IntersectTriangle(ray, bestHit, v2, v3, v1, albedo, emission, specular, smooth, refractEta);
}
void IntersectCubeT

	(Ray

	ray, inout
	Hit bestHit, float3 pos, float3 size, float3 albedo, float3 emission, float3 specular = 0, float smooth = 0, float4 refractEta = 0)
{
	IntersectQaud(ray, bestHit, float3(-size.x, 0, -size.z) + pos, float3(size.x, 0, -size.z) + pos, float3(size.x, 0, size.z) + pos, float3(-size.x, 0, size.z) + pos, 0.8f, 0, 0, 0); // Пол
	IntersectQaud(ray, bestHit, float3(size.x, 0, size.z) + pos, float3(size.x, 0, -size.z) + pos, float3(size.x, size.y * 2, -size.z) + pos, float3(size.x, size.y * 2, size.z) + pos, Colorizate(109, 102, 172), 0, 0, 0); // Право
	IntersectQaud(ray, bestHit, float3(-size.x, 0, -size.z) + pos, float3(-size.x, 0, size.z) + pos, float3(-size.x, size.y * 2, size.z) + pos, float3(-size.x, size.y * 2, -size.z) + pos, Colorizate(175, 101, 111), 0, 0, 0); // Лево
	IntersectQaud(ray, bestHit, float3(-size.x, size.y * 2, size.z) + pos, float3(size.x, size.y * 2, size.z) + pos, float3(size.x, size.y * 2, -size.z) + pos, float3(-size.x, size.y * 2, -size.z) + pos, 0.8f, 0, 0, 0); // Верх
	IntersectQaud(ray, bestHit, float3(-size.x, 0, size.z) + pos, float3(size.x, 0, size.z) + pos, float3(size.x, size.y * 2, size.z) + pos, float3(-size.x, size.y * 2, size.z) + pos, 0.8f, 0, 0, 0); // Перед
	IntersectQaud(ray, bestHit, float3(size.x, 0, -size.z) + pos, float3(-size.x, 0, -size.z) + pos, float3(-size.x, size.y * 2, -size.z) + pos, float3(size.x, size.y * 2, -size.z) + pos, 0.05f, 0, 0, 0); // Зад
}
void IntersectPrisma

	(Ray

	ray, inout
	Hit bestHit, float3 pos, float3 size, float3 albedo, float3 emission, float3 specular = 0, float smooth = 0, float4 refractEta = 0)
{
	IntersectQaud(ray, bestHit, float3(-size.x, 0, -size.z) + pos, float3(size.x, 0, -size.z) + pos, float3(size.x, 0, size.z) + pos, float3(-size.x, 0, size.z) + pos, 0, 0, 0, 0, refractEta); // Пол
	IntersectQaud(ray, bestHit, float3(size.x, 0, size.z) + pos, float3(size.x, 0, -size.z) + pos, float3(size.x, size.y * 2, -size.z) + pos, float3(size.x, 0, size.z) + pos, 0, 0, 0, 0, refractEta); // Право
	IntersectQaud(ray, bestHit, float3(-size.x, 0, -size.z) + pos, float3(-size.x, 0, size.z) + pos, float3(-size.x, 0, size.z) + pos, float3(-size.x, size.y * 2, -size.z) + pos, 0, 0, 0, 0, refractEta); // Лево
	IntersectQaud(ray, bestHit, float3(-size.x, 0, size.z) + pos, float3(size.x, 0, size.z) + pos, float3(size.x, size.y * 2, -size.z) + pos, float3(-size.x, size.y * 2, -size.z) + pos, 0, 0, 0, 0, refractEta); // Верх
	IntersectQaud(ray, bestHit, float3(size.x, 0, -size.z) + pos, float3(-size.x, 0, -size.z) + pos, float3(-size.x, size.y * 2, -size.z) + pos, float3(size.x, size.y * 2, -size.z) + pos, 0, 0, 0, 0, refractEta); // Зад
}
bool IntersectCubeBase(Ray ray, float3 min, float3 max, int cullMode, inout float t)
{
	float t0 = 0;
	float t1 = 0;
	
	float t0Y = 0;
	float t1Y = 0;
	
	float t0Z = 0;
	float t1Z = 0;
	

	if (ray.direction.x > 0)
	{
		t0 = (min.x - ray.origin.x) / ray.direction.x;
		t1 = (max.x - ray.origin.x) / ray.direction.x;
	}
	else if (ray.direction.x < 0)
	{
		t0 = (max.x - ray.origin.x) / ray.direction.x;
		t1 = (min.x - ray.origin.x) / ray.direction.x;
	}

	
	if (ray.direction.y > 0)
	{
		t0Y = (min.y - ray.origin.y) / ray.direction.y;
		t1Y = (max.y - ray.origin.y) / ray.direction.y;
	}
	else if (ray.direction.y < 0)
	{
		t0Y = (max.y - ray.origin.y) / ray.direction.y;
		t1Y = (min.y - ray.origin.y) / ray.direction.y;
	}
	if (t0 > t1Y || t0Y > t1)
		return false;
	
	if (t0Y > t0)
	{
		t0 = t0Y;
	}
	
	if (t1Y < t1)
	{
		t1 = t1Y;
	}
	
	if (ray.direction.z > 0)
	{
		t0Z = (min.z - ray.origin.z) / ray.direction.z;
		t1Z = (max.z - ray.origin.z) / ray.direction.z;
	}
	else if (ray.direction.z < 0)
	{
		t0Z = (max.z - ray.origin.z) / ray.direction.z;
		t1Z = (min.z - ray.origin.z) / ray.direction.z;
	}
	if (t0 > t1Z || t0Z > t1)
		return false;
		
	if (t0Z > t0)
	{
		t0 = t0Z;
	}
	if (t1Z < t1)
	{
		t1 = t1Z;
	}

	if (cullMode == 0)
	{
		t = t0;
		return true;
	}
	if (cullMode == 1)
	{
		t = t1;
		return true;
	}
	if (cullMode == 2)
	{
		if (t0 < 0)
		{
			t = t1;
			
		}
		else
		{
			t = t0;
		}
	}
	return true;
}

bool IntersectCube(Ray ray, inout Hit bestHit, float3 min, float3 max, float3 albedo)
{
	float t = 0;
	IntersectCubeBase(ray, min, max, 2, t);
	if (t > 0 & t < bestHit.t)
	{
		bestHit.t = t;
		bestHit.position = ray.origin + t * ray.direction;
		bestHit.normal = float3(1, 0, 0);
		bestHit.albedo = albedo;
		bestHit.emission = 0;
		bestHit.specular = 0;
		bestHit.smooth = 0;
		return true;
	}
	return false;
}

bool checkEqualsVolume(float3 checkedVolumeMin, float3 checkedVolumeMax, float3 volumeMin, float3 volumeMax)
{

	if (checkedVolumeMin.x <= volumeMin.x & checkedVolumeMax.x >= volumeMin.x || checkedVolumeMin.x <= volumeMax.x & checkedVolumeMax.x >= volumeMax.x || checkedVolumeMin.x >= volumeMin.x & checkedVolumeMax.x <= volumeMax.x)
		if (checkedVolumeMin.y <= volumeMin.y & checkedVolumeMax.y >= volumeMin.y || checkedVolumeMin.y <= volumeMax.y & checkedVolumeMax.y >= volumeMax.y || checkedVolumeMin.y >= volumeMin.y & checkedVolumeMax.y <= volumeMax.y)
			if (checkedVolumeMin.z <= volumeMin.z & checkedVolumeMax.z >= volumeMin.z || checkedVolumeMin.z <= volumeMax.z & checkedVolumeMax.z >= volumeMax.z || checkedVolumeMin.z >= volumeMin.z & checkedVolumeMax.z <= volumeMax.z)
				return true;
	return false;
}

bool splitedCubeIntersect(Ray ray, inout Hit bestHit, float3 checkedVolumeMin, float3 checkedVolumeMax, float3 volumeMin, float3 volumeMax, int extent)
{
	//if ()
	//	IntersectCube(ray, bestHit, volumeMin, volumeMax / 2);
	return false;
}

bool BVHCheck(Ray ray, inout Hit bestHit, float3 checkedVolumeMin, float3 checkedVolumeMax, float3 firstVolumeSize)
{
	float3 originPoint = float3(0, 0, 0);
	bool intersectCheck = false;
	intersectCheck = splitedCubeIntersect(ray, bestHit, checkedVolumeMin, checkedVolumeMax, originPoint - firstVolumeSize, originPoint + firstVolumeSize, 2);
	return intersectCheck;
}
Hit Trace(Ray ray, inout float seed)
{
	Hit hit = InitHit();
	float3 intersectColor = Colorizate(139, 255, 179);
	float3 noIntersectColor = Colorizate(255, 15, 31);
	
	float3 volumeMin0 = float3(-1, -1, -1);
	float3 volumeMax0 = float3(1, 1, 1);
	IntersectCube(ray, hit, volumeMin0, volumeMax0, 0.8f);
	
	float3 volumeMin1 = float3(0, 0, sphere1Pos.x * 2);
	float3 volumeMax1 = float3(1, 1, sphere1Pos.z * 2 + 1);
	float3 color = 0;
	if (checkEqualsVolume(volumeMin1, volumeMax1, volumeMin0, volumeMax0))
	{
		color = intersectColor;
	}
	else
		color = noIntersectColor;
	IntersectCube(ray, hit, volumeMin1, volumeMax1, color);
	
	//IntersectMesh(ray, hit, 0);
	//CheckIntersection(ray, hit, float3(1, 1, 1), 20);

	//IntersectFlat(ray, hit, float3(0, 0, 0), float3(0, 1, 0), 0.8f, 0);
		return hit;
	}
	float energy

	(

	float3 color)
{
	//return color.x + color.y + color.z;
		return dot(color, 1.0f / 3.0f);
	}
	float fresnel

	(Ray

	ray, Hit

	hit)
{
		float kr = 0;
		float cosi = clamp(-1, 1, dot(ray.direction, hit.normal));
		float etai = 1;
		float etat = hit.eta.w;
		if (cosi > 0)
		{
			etat = etai;
			etai = hit.eta.w;
		}
    // Compute sini using Snell's law
		float sint = etai / etat * sqrt(max(0.f, 1 - cosi * cosi));
    // Total internal reflection
		if (sint >= 1)
		{
			return kr = 1;
		}
		else
		{
			float cost = sqrt(max(0.f, 1 - sint * sint));
			cosi = abs(cosi);
			float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
			float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
			return kr = (Rs * Rs + Rp * Rp) / 2;
		}
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
	}
	float3 refractScratchPixel

	(

	float3 incedent, float3 normal, float ior)
{
		float cosi = clamp(-1, 1, dot(incedent, normal));
		float etai = 1, etat = ior;
		float3 n = normal;
		if (cosi < 0)
		{
			cosi = -cosi;
		}
		else
		{
			etat = etai;
			etai = ior;
			n = -normal;
		}
		float eta = etai / etat;
		float k = 1 - eta * eta * (1 - cosi * cosi);
		return k < 0 ? 0 : eta * incedent + (eta * cosi - sqrt(k)) * n;
	}
	float3 Albedo

	(inout
	Ray ray, Hit

	hit,

	float chance, float bias)
{
		ray.origin = hit.position + hit.normal * bias;
		ray.direction = SampleHemisphere(hit.normal, 1);
		return ((1.0f / chance) * hit.albedo);
	}
	float3 Specular

	(inout
	Ray ray, Hit

	hit,

	float chance, float bias)
{
		ray.origin = hit.position + hit.normal * bias;
		ray.direction = SampleHemisphere(reflect(ray.direction, hit.normal), hit.smooth);
		float f = (hit.smooth + 2) / (hit.smooth + 1);
		return (1.0f / chance) * hit.specular * pow(sdot(hit.normal, ray.direction, f), 0.1f);
	}
	float3 Refraction

	(inout
	Ray ray, Hit

	hit,

	float chance, float bias)
{
		float kr = fresnel(ray, hit);
		float tr = 1 - kr;
		float fresnelRand = rand();
		if (fresnelRand < kr)
		{
			float smooth = 10000000;
			ray.origin = hit.position + hit.normal * bias;
			ray.direction = SampleHemisphere(reflect(ray.direction, hit.normal), smooth);
			float f = (smooth + 2) / (smooth + 1);
			return (1.0f / chance) * float3(1, 1, 1);
		}
		else
		{
			ray.origin = hit.position - hit.normal * bias;
			ray.direction = refractScratchPixel(ray.direction, hit.normal, hit.eta.w);
			return (1.0f / chance) * hit.eta.xyz;
		}
	}
	float3 Refraction2

	(inout
	Ray ray, Hit

	hit,

	float chance, float bias)
{
		ray.origin = hit.position - hit.normal * bias;
		ray.direction = refractScratchPixel(ray.direction, hit.normal, hit.eta.w);
		return (1.0f / chance) * hit.eta.xyz;
	}

	float3 Shade

	(inout
	Ray ray, Hit

	hit)
{
		if (hit.t < 1.#INF)
		{
			hit.albedo = min(1.0f - hit.specular, hit.albedo);
			float sum = energy(hit.specular) + energy(hit.albedo) + energy(hit.eta.xyz);
			float bias = 0.00015f;
			if (sum == 0)
			{
				hit.albedo = 1;
				ray.energy *= Albedo(ray, hit, 1, bias);
				return hit.emission;
			}
			float chanceSpecular = energy(hit.specular) / sum;
			float chanceRefract = energy(hit.eta.xyz) / sum;
			float chanceAlbedo = energy(hit.albedo) / sum;
			float random = rand();
			if (random < chanceAlbedo)
			{
				ray.energy *= Albedo(ray, hit, chanceAlbedo, bias);
			}
			else if (random >= chanceAlbedo & random < chanceSpecular)
			{
				ray.energy *= Specular(ray, hit, chanceSpecular, bias);
			}
			else if (random >= chanceSpecular & random < chanceRefract)
			{
				ray.energy *= Refraction(ray, hit, chanceRefract, bias);
			}
		//else
		//{
		//	hit.albedo = 1;
		//	ray.energy *= Albedo(ray, hit, 1, bias);
		//}
			return hit.emission;
		}
		else
		{
			ray.energy = 0.0f;
			float theta = acos(ray.direction.y);
			float phi = atan2(ray.direction.x, ray.direction.z);
			return HDRtexture.SampleLevel(objSamplerState, float2(phi / (2 * PI), theta / PI), 0);
		}
	}
	float3 initPixelPos

	(

	float3 pixelPos, float2 cameraAngle, float viewingAngle, float screenResolutionCoefficient)
{
		float3 pos = float3(0, 0, 0);
		pos.x = (viewingAngle * sin(cameraAngle.x) * cos(cameraAngle.y)) + pixelPos.x * screenResolutionCoefficient * cos(cameraAngle.x) - pixelPos.y * sin(cameraAngle.y) * sin(cameraAngle.x);
		pos.y = viewingAngle * sin(cameraAngle.y) + pixelPos.y * cos(cameraAngle.y);
		pos.z = viewingAngle * cos(cameraAngle.x) * cos(cameraAngle.y) - pixelPos.x * screenResolutionCoefficient * sin(cameraAngle.x) - pixelPos.y * sin(cameraAngle.y) * cos(cameraAngle.x);
		return pos;
	}

	void intersectCyrcle

	(inout
	float4 result, float2 pos, float radius, float4 albedo)
{
		if (length(float2(_PixelPos.x * screenResolution.w, _PixelPos.y) - pos) <= radius)
			result = albedo * (1 - length(float2(_PixelPos.x * screenResolution.w, _PixelPos.y) - pos) / radius);
		return;
	}

float4 main(Input input) : SV_TARGET
{
		_PixelPos = input.pixelPos;
		_Seed = random1Value.x;
		_Seed1 = randomValue.x;
		float3 result = 0;
		uint iterCount = 8;
		float3 viewDiraction = 0;
		viewDiraction = initPixelPos(_PixelPos + float3((rand() * 2) - 1, (rand() * 2) - 1, (rand() * 2) - 1) * 0.0012f, float2(cameraAngle.x, cameraAngle.y), 1, screenResolution.w);
		Ray ray = InitRay(normalize(viewDiraction), cameraPos);
		float seed = 256;
		for (int i = 0; i < iterCount; i++)
		{
			Hit hit = Trace(ray, seed);
			result += ray.energy * Shade(ray, hit);
			if (!any(ray.energy))
				break;
		}

		if (k.w == 0)
		{
		//result.x = Lerp3(objTexture.Sample(objSamplerState, input.textureCoord).x, result.x, 1 / (samplesCount.y + 1));
		//result.y = Lerp(objTexture.Sample(objSamplerState, input.textureCoord).y, result.y, 1 / (samplesCount.y + 1));
		//result.z = Lerp(objTexture.Sample(objSamplerState, input.textureCoord).z, result.z, 1 / (samplesCount.y + 1));
			result = (result + objTexture.Sample(objSamplerState, input.textureCoord) * (samplesCount.y - 1)) / samplesCount.y;
		}
		return float4(result, 1);
	}

#pragma once
#include "Object_PT.h"
#include "float3.h"
#include <vector>
class Sphere_PT : public Object_PT
{
public:
	Sphere_PT(std::vector<Object_PT*>& objectsArray, float radius = 0, float3 position = {}, float4 albedo = {}, float4 emission = {});
	struct SpherePrimitive
	{
		float3 position;
		float radius;
		int objID;
	};
	float3 position;
	float radius;
};
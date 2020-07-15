#pragma once
#include <vector>
#include "float3.h"

class Object_PT
{
public:
	enum class ObjectType_PT
	{
		Model = 0,
		Sphere = 1,
		Camera = 2
	};
	ObjectType_PT type;
	float4 albedo;
	float4 emission;
};


#pragma once
#include "Primitive.h"
#include <vector>
#include "float3.h"

class RTObject
{
public:
	enum class RTObjectType
	{
		Model = 0,
		Sphere = 1,
		Camera = 2
	};
	RTObjectType type;
};


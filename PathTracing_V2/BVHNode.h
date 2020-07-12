#pragma once
#include "float3.h"
struct BVHNode 
{
	float3 MinBound;
	float3 MaxBound;
	size_t primiteveOffset;
	size_t nPrimiteve;
};
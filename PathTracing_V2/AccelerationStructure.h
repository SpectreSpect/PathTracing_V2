#pragma once
#include <d3d11.h>
#include "Primitive.h"
#include <vector>
class AccelerationStructure
{
public:
	virtual HRESULT Build(std::vector<Primitive> &primitiveBuffer, void* pNodeArray) = 0;
};


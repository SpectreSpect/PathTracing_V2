#pragma once
#include "AccelerationStructure.h"
class BVH : public AccelerationStructure
{
public:
	virtual HRESULT Build(std::vector<Primitive>& primitiveBuffer, void* pNodeArray) override;
};


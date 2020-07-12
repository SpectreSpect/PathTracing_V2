#pragma once

struct Primitive 
{
	enum PrimitiveType
	{
		Triangle = 0,
		Sphere = 1,
		Box = 2
	};
	unsigned int ObjID;
	unsigned int primitiveID;
	PrimitiveType primitiveType;
};
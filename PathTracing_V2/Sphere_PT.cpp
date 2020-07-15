#include "Sphere_PT.h"

Sphere_PT::Sphere_PT(std::vector<Object_PT*>& objectsArray, float radius, float3 position, float4 albedo, float4 emission)
{
	type = Object_PT::ObjectType_PT::Sphere;
	this->radius = radius;
	this->position = position;
	this->albedo = albedo;
	this->emission = emission;
	objectsArray.push_back(this);
}
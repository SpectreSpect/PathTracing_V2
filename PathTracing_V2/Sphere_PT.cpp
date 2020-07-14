#include "Sphere_PT.h"

Sphere_PT::Sphere_PT(std::vector<Object_PT*>& objectsArray, float radius, float3 position, float3 albedo)
{
	type = Object_PT::ObjectType_PT::Sphere;
	this->radius = radius;
	this->position = position;
	this->albedo = albedo;
	objectsArray.push_back(this);
}
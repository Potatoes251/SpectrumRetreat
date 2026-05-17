#ifndef COLISION3D_H
#define COLISION3D_H

#include "Geometry3D.h"
#include "Vector/Vector3.h"

namespace LibMath
{
	//float g_epsilon = 1e-6;
	//3D collision

	bool			collisionLinePlane(Line3D, Plane);
	bool			collisionTwoPlane(Plane, Plane);

	bool			collisionPointSphere(Point3D, Sphere);
	bool			collisionRaySphere(Line3D, Sphere);
	bool			collisionPlaneSphere(Plane, Sphere);
	bool			collisionTwoSphere(Sphere, Sphere);

	bool			collisionPointCapsule(Point3D, Capsule);
	bool			collisionRayCapsule(Line3D, Capsule);
	bool			collisionPlaneCapsule(Plane, Capsule);	// todo
	bool			collisionSphereCapsule(Sphere, Capsule);
	bool			collisionTwoCapsule(Capsule, Capsule);	// todo

	//void			getMinMax(float& min, float& max, Point3D corners[4], Vector2 axis);
	//void			getNormals(Vector2 axes[8], Point3D corners1[4], Point3D corners2[4]);

	bool			collisionPointPrism(Point3D, Prism);					// aabb
	bool			collisionRayPrism(Line3D, Prism);						// aabb
	bool			intersectionRayPrism(Line3D, Prism, Vector3&);			// aabb		return the first touching point
	bool			collisionSpherePrism(Sphere, Prism);					// obb		todo
	bool			collisionCapsulePrism(Capsule, Prism);					// aabb
	bool			intersectionCapsulePrism(Capsule, Prism, Vector3&, float&);		// aabb		return the normal and first correction to apply to make them not touch
	bool			collisionTwoPrism(Prism, Prism);						// obb		todo
}


#endif
#include "LibMath/Colision3D.h"


bool	LibMath::collisionLinePlane(Line3D line, Plane plane)
{
	return line.getDirection().dot(plane.getNormal()) != 0;
}

bool	LibMath::collisionTwoPlane(Plane plane1, Plane plane2)
{
	return plane1.getNormal() != plane2.getNormal();
}


bool	LibMath::collisionPointSphere(Point3D point, Sphere sphere)
{
	return sphere.getCenter().distanceSquare(point) - sphere.getRadius() > g_epsilon;
}

bool	LibMath::collisionRaySphere(Line3D line3D, Sphere sphere)
{
	Vector3 m = line3D.getCenter() - sphere.getCenter();

	float b = m.dot(line3D.getDirection());
	float c = m.dot(m) - sphere.getRadius() * sphere.getRadius();
	return 0 <= (b * b * c);
}

bool	LibMath::collisionPlaneSphere(Plane plane, Sphere sphere)
{
	return plane.getPointOnPlane().distance(sphere.getCenter()) < sphere.getRadius();
}

bool	LibMath::collisionTwoSphere(Sphere sphere1, Sphere sphere2)
{
	return sphere1.getCenter().distanceSquare(sphere2.getCenter()) - (sphere1.getRadius() + sphere2.getRadius()) > g_epsilon;
}

bool	LibMath::collisionPointCapsule(Point3D point, Capsule capsule)
{
	LibMath::Vector3 capsuleSeg = capsule.getEnd() - capsule.getStart();
	LibMath::Vector3 AP = point - capsule.getStart();

	float t = capsuleSeg.dot(AP) / capsuleSeg.magnitudeSquared();

	t = LibMath::clamp(t, 0.f, 1.f);

	float distance = (point - (capsule.getStart() + capsuleSeg * t)).magnitudeSquared();

	return distance < capsule.getRadius() * capsule.getRadius();
}

bool	LibMath::collisionRayCapsule(Line3D ray, Capsule capsule)
{
	LibMath::Vector3 capsuleSegment = capsule.getEnd() - capsule.getStart();
	LibMath::Vector3 AP = ray.getCenter() - capsule.getStart();

	float u = capsuleSegment.dot(AP) / capsuleSegment.magnitudeSquared();
	u = LibMath::clamp(u, 0.f, 1.f);

	LibMath::Vector3 capPoint = capsule.getStart() + capsuleSegment * u;

	float t = (capsule.getStart() + (-ray.getCenter())).dot(ray.getDirection());
	t = t < 0 ? 0 : t;

	LibMath::Vector3 rayPoint = ray.getCenter() + ray.getDirection() * t;

	float distance = capPoint.distanceSquaredFrom(rayPoint);

	if (distance < capsule.getRadius() * capsule.getRadius())
	{
		return true;
	}

	return collisionRaySphere(ray, Sphere(capsule.getStart(), capsule.getRadius())) ||
			collisionRaySphere(ray, Sphere(capsule.getEnd(), capsule.getRadius()));
}

bool	LibMath::collisionSphereCapsule(Sphere sphere, Capsule capsule)
{
	LibMath::Vector3 AB = capsule.getEnd() - capsule.getStart();
	LibMath::Vector3 AP = sphere.getCenter() - capsule.getStart();

	float t = AB.dot(AP) / AB.magnitudeSquared();

	t = LibMath::clamp(t, 0.f, 1.f);

	float distance = (sphere.getCenter() - (capsule.getStart() + AB * t)).magnitudeSquared();
	float radiusSum = capsule.getRadius() + sphere.getRadius();

	return distance < radiusSum * radiusSum;
}

bool LibMath::collisionPointPrism(Point3D point, Prism prism)
{
	return (point.getX() <= prism.getCenter().getX() + prism.extentX() &&
			point.getY() <= prism.getCenter().getY() + prism.extentY() &&
			point.getZ() <= prism.getCenter().getZ() + prism.extentZ() &&
			point.getX() >= prism.getCenter().getX() &&
			point.getY() >= prism.getCenter().getY() &&
			point.getZ() >= prism.getCenter().getZ());
}
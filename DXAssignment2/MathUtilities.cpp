#include "MathUtilities.h"


/*
Summary:
	Constructor: Create a bounding box the size of negative 'infinity.' 
Params: -
Return: -
*/
BoundingBox::BoundingBox() {
	_min.x = FLT_MAX;
	_min.y = FLT_MAX;
	_min.z = FLT_MAX;

	_max.x = -FLT_MAX;
	_max.y = -FLT_MAX;
	_max.z = -FLT_MAX;
}

/*
Summary:
	Tests whether a point is inside this bounding box and returns true if it is.
Params:
	p: The 3D point to test inside-ness against.
Return: True if the point p is contained within the BoundingBox.
*/
bool BoundingBox::isPointInside(D3DXVECTOR3 & p) {
	return p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
		p.x <= _max.x && p.y <= _max.y && p.z <= _max.z;
}

/*
Summary:
	Constructor: Creates a BoundingSphere with a default radius of 1.0f.
Params: -
Return: -
*/
BoundingSphere::BoundingSphere() { 
	_radius = 1.0f; 
}

/*
Summary:
	Transforms a position in x/y screen coordinates into a ray into a x/y/z ray in camera space.  
	To convert the ray to world coordinates, transform the resulting ray by the inverse of the camera view matrix.
Params:
	device: The Direct3D device the picking is being done in.
	projection: the projection matrix of the camera being picked through.
	x: The x screen coordinate to pick at (in pixels, top left is (0,0))
	y: The y screen coordinate to pick at (in pixels, top left is (0,0))
Return: The ray calculated, pointing from the origin, at an angle based on the camera.
*/
Ray MathUtilities::CalcPickingRay(LPDIRECT3DDEVICE9 & device, const D3DXMATRIX & projection, int x, int y) {
	float px = 0.0f;
	float py = 0.0f;

	D3DVIEWPORT9 vp;
	device->GetViewport(&vp);

	px = (((2.0f*x) / vp.Width) - 1.0f) / projection(0, 0);
	py = (((-2.0f*y) / vp.Height) + 1.0f) / projection(1, 1);

	Ray ray;
	ray._origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray._direction = D3DXVECTOR3(px, py, 1.0f);

	return ray;
}

/*
Summary:
	Transform the origin and the direction of the ray based on the given transformation matrix.
Params:
	ray: The ray to transform.
	T: The transformation to apply to the ray.
Return: -
*/
void MathUtilities::TransformRay(Ray & ray, const D3DXMATRIX & T) {
	// transform the ray's origin, w = 1.
	D3DXVec3TransformCoord(
		&ray._origin,
		&ray._origin,
		&T);

	// transform the ray's direction, w = 0.
	D3DXVec3TransformNormal(
		&ray._direction,
		&ray._direction,
		&T);

	// normalize the direction
	D3DXVec3Normalize(&ray._direction, &ray._direction);
}

/*
Summary:
	Test for intersection between a ray and a BoundingSphere, 
	given the same coordinate system.
Params:
	ray: The ray to hit-test with.
	T: The sphere to test for intersections with.
Return: The distance along the ray at the point of intersection with the sphere. Negative numbers indicate no hits.
*/
float MathUtilities::RaySphereIntTest(const Ray & ray, const BoundingSphere & sphere) {
	D3DXVECTOR3 v = ray._origin - sphere.center_;

	float b = 2.0f * D3DXVec3Dot(&ray._direction, &v);
	float c = D3DXVec3Dot(&v, &v) - (sphere._radius * sphere._radius);

	// find the discriminant
	float discriminant = (b * b) - (4.0f * c);

	// test for imaginary number
	if (discriminant < 0.0f)
		return -1.0f;

	discriminant = sqrtf(discriminant);

	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;

	// if a solution is >= 0, then we intersected the sphere
	if (s0 >= 0.0f || s1 >= 0.0f) {
		return 1.0f;

		return s0 < s1 && s0 >= 0.0f? s0 : s1;
	}

	return -1.0f;
}

#include "MathUtilities.h"

BoundingBox::BoundingBox() {
	_min.x = FLT_MAX;
	_min.y = FLT_MAX;
	_min.z = FLT_MAX;

	_max.x = -FLT_MAX;
	_max.y = -FLT_MAX;
	_max.z = -FLT_MAX;
}

bool BoundingBox::isPointInside(D3DXVECTOR3 & p) {
	return p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
		p.x <= _max.x && p.y <= _max.y && p.z <= _max.z;
}

BoundingSphere::BoundingSphere() { 
	_radius = 1.0f; 
}

Ray MathUtilities::CalcPickingRay(LPDIRECT3DDEVICE9 & device, int x, int y) {
	float px = 0.0f;
	float py = 0.0f;

	D3DVIEWPORT9 vp;
	device->GetViewport(&vp);

	D3DXMATRIX proj;
	device->GetTransform(D3DTS_PROJECTION, &proj);

	px = (((2.0f*x) / vp.Width) - 1.0f) / proj(0, 0);
	py = (((-2.0f*y) / vp.Height) + 1.0f) / proj(1, 1);

	Ray ray;
	ray._origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray._direction = D3DXVECTOR3(px, py, 1.0f);

	return ray;
}

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

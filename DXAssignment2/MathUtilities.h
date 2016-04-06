#pragma once

#include <d3dx9.h>

struct BoundingBox {
	BoundingBox();

	bool isPointInside(D3DXVECTOR3& p);

	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;
};

struct BoundingSphere {
	BoundingSphere();

	D3DXVECTOR3 center_;
	float       _radius;
};

struct Ray {
	D3DXVECTOR3 _origin;
	D3DXVECTOR3 _direction;
};

struct MathUtilities {
	static Ray CalcPickingRay(LPDIRECT3DDEVICE9& device, const D3DXMATRIX & projection, int x, int y);
	static void TransformRay(Ray& ray, const D3DXMATRIX& T);
	static float RaySphereIntTest(const Ray& ray, const BoundingSphere& sphere);
};

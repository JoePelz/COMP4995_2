#include "MirrorCube.h"



MirrorCube::MirrorCube() {
}


MirrorCube::~MirrorCube() {
}

void MirrorCube::setFace(int face) {
	face_ = face;
	//TODO: set facePlane_

	// D3DXPLANE {x, y, z, d} 
	// x, y, z are the normal of the plane
	// d is negative distance from origin
	// 0, 1, 0, 100 is a plane that passes through (0, -100, 0)
	D3DXVECTOR3 tempVec;
	switch (face) {
	case 0:
		tempVec = { 0, 0, -1 }; // +
		break;
	case 1:
		tempVec = { 1, 0, 0 }; // -
		break;
	case 2: default:
		tempVec = { 0, 1, 0 }; // -
		break;
	case 3:
		tempVec = { 0, 0, 1 }; // -
		break;
	case 4:
		tempVec = { -1, 0, 0 }; // +
		break;
	case 5:
		tempVec = { 0, -1, 0 }; // +
		break;
	}
	//D3DXVec4Transform((D3DXVECTOR4*)&facePlane_, &tempVec, &getTransform());
	D3DXVECTOR3 normal, pos;
	D3DXVec3TransformNormal(&normal, &tempVec, &getTransform());
	D3DXVec3TransformCoord(&pos, &tempVec, &getTransform());
	float dist = D3DXVec3Dot(&normal, &pos);

	//distance is the dot product of a vertex and the normal
	facePlane_ = { normal.x, normal.y, normal.z, -dist };
	D3DXMatrixReflect(&reflection_, &facePlane_);
	facePlane_.a = -facePlane_.a;
	facePlane_.b = -facePlane_.b;
	facePlane_.c = -facePlane_.c;
	facePlane_.d = -facePlane_.d;

}

void MirrorCube::draw(LPDIRECT3DDEVICE9 & device, const D3DXMATRIX * xform, int face) {
	device->SetFVF(ColouredNormalVertex_FLAGS);
	device->SetStreamSource(0, vertexBuffer_, 0, ColouredNormalVertex_STRIDE);
	/*
	const D3DXMATRIX* T;
	if (xform) {
		T = &getReflection(*xform);
	} else {
		T = &getTransform();
	}
	*/
	device->SetRenderState(D3DRS_COLORVERTEX, false);
	if (xform) {
		device->SetTransform(D3DTS_WORLD, &getReflection(*xform));
	} else {
		device->SetTransform(D3DTS_WORLD, &getTransform());
	}
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, face * 4, 2);
}

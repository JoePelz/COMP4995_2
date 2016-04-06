#include "MirrorCube.h"


/*
Summary:
	Constructor: create a new cube to be used as a mirror surface.
Params: -
Return: -
*/
MirrorCube::MirrorCube() {
	//material is Diffuse, Ambient, Specular, Emissive, Power(float
	//colors are r, g, b, a
	material_.Diffuse =  { 0.0f, 0.0f, 0.0f, 1.0f };
	material_.Ambient =  { 0.0f, 0.0f, 0.0f, 1.0f };
	material_.Emissive = { 0.0f, 0.0f, 0.0f, 1.0f };
	material_.Specular = { 0.0f, 0.0f, 0.0f, 1.0f };
	material_.Power = 1.0f;
}

MirrorCube::~MirrorCube() {
}

/*
Summary:
	Activate a particular face of the cube for usage. Affects getFacePlane and getFaceReflection.
Params: 
	face: The face to activate. Must be [0..5] or behaviour is undefined.
Return: -
*/
void MirrorCube::setFace(int face) {
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
	D3DXVECTOR3 normal, pos;
	D3DXVec3TransformNormal(&normal, &tempVec, &getTransform());
	D3DXVec3TransformCoord(&pos, &tempVec, &getTransform());
	float dist = D3DXVec3Dot(&normal, &pos);

	//distance is the dot product of a transformed vertex and the normal
	facePlane_ = { normal.x, normal.y, normal.z, -dist };
	D3DXMatrixReflect(&reflection_, &facePlane_);
	facePlane_.a = -facePlane_.a;
	facePlane_.b = -facePlane_.b;
	facePlane_.c = -facePlane_.c;
	facePlane_.d = -facePlane_.d;

}

/*
Summary:
	Render an individual face (quad) of the cube in the 3D context.
Params: 
	device: The device context to render into.
	xform: Any additional transformation to apply to the face.
	face: The face to draw. Must be [0..5] or behaviour is undefined.
Return: -
*/
void MirrorCube::drawFace(LPDIRECT3DDEVICE9 & device, const D3DXMATRIX * xform, int face) {
	device->SetFVF(ColouredNormalVertex_FLAGS);
	device->SetStreamSource(0, vertexBuffer_, 0, ColouredNormalVertex_STRIDE);
	device->SetRenderState(D3DRS_COLORVERTEX, false);

	if (xform) {
		device->SetTransform(D3DTS_WORLD, &getReflection(*xform));
	} else {
		device->SetTransform(D3DTS_WORLD, &getTransform());
	}
	device->SetMaterial(&material_);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, face * 4, 2);
}

/*
Summary:
	Draw the vertices of the cube to screen as squares.
Params:
	device: The device context to render into.
	xform: Any additional transformation to apply to the vertices.
Return: -
*/
void MirrorCube::drawVerts(LPDIRECT3DDEVICE9 & device, const D3DXMATRIX * xform) {
	device->SetFVF(ColouredNormalVertex_FLAGS);
	device->SetStreamSource(0, vertexBuffer_, 0, ColouredNormalVertex_STRIDE);
	if (xform) {
		device->SetTransform(D3DTS_WORLD, &getReflection(*xform));
	} else {
		device->SetTransform(D3DTS_WORLD, &getTransform());
	}
	float pointSize = 10.0f;
	device->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&pointSize));
	device->DrawPrimitive(D3DPT_POINTLIST, 0, 12);
}

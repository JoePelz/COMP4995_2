#pragma once

#include "Cube.h"

class MirrorCube : public Cube {
	D3DXPLANE facePlane_;
	D3DXMATRIX reflection_;
	D3DMATERIAL9 material_;
public:
	MirrorCube();
	~MirrorCube();
	void setFace(int face);
	void draw(LPDIRECT3DDEVICE9& device, const D3DXMATRIX* xform, int face);
	void drawVerts(LPDIRECT3DDEVICE9& device, const D3DXMATRIX* xform);
	inline const D3DXPLANE& getFacePlane() { return facePlane_; }
	inline const D3DXMATRIX& getFaceReflection() { return reflection_; }
};


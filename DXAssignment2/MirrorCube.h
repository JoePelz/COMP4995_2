#pragma once

#include "Cube.h"

class MirrorCube : public Cube {
	int face_;
	D3DXPLANE facePlane_;
	D3DXMATRIX reflection_;
public:
	MirrorCube();
	~MirrorCube();
	void setFace(int face);
	void draw(LPDIRECT3DDEVICE9& device, const D3DXMATRIX* xform, int face);
	inline const D3DXPLANE& getFacePlane() { return facePlane_; }
	inline const D3DXMATRIX& getFaceReflection() { return reflection_; }
};


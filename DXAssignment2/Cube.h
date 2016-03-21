#pragma once

#include "Drawable3D.h"
#include "Errors.h"

//Flexible Vertex Format definition for position, normals, and color

#define ColouredNormalVertex_FLAGS D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE
#define ColouredNormalVertex_STRIDE 28
struct ColouredNormalVertex {
	float x, y, z;
	float nx, ny, nz;
	DWORD colour;
};

/*
The Cube class represents a 3D Cube with colored-vertex sides that can be rendered.
*/

class Cube : public Drawable3D {
protected:
	IDirect3DVertexBuffer9* vertexBuffer_; //pointer to buffer within the Direct3D device.
	D3DXMATRIX net;
public:
	void initializeResources(LPDIRECT3DDEVICE9& device) override;
	void releaseResources() override;
	void draw(LPDIRECT3DDEVICE9& device, const D3DXMATRIX* xform) override;
	Cube() = default;
	~Cube() = default;
};


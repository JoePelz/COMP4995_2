#pragma once

#include "Drawable3D.h"
#include "Errors.h"

#define ColouredNormalVertex_FLAGS D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE
#define ColouredNormalVertex_STRIDE 28
struct ColouredNormalVertex {
	float x, y, z;
	float nx, ny, nz;
	DWORD colour;
};

class Cube :
	public Drawable3D{
	IDirect3DVertexBuffer9* vertexBuffer_;
	D3DMATERIAL9 material_;
public:
	void initializeResources(LPDIRECT3DDEVICE9& device) override;
	void releaseResources() override;
	void draw(LPDIRECT3DDEVICE9& device) override;
	Cube();
	~Cube();
};


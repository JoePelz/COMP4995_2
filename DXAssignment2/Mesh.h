#pragma once

#include <Windows.h>
#include <d3d9.h>
#include "Drawable3D.h"
#include "Errors.h"
#include "atlstr.h"

struct ColouredVertex {
	float x, y, z;
	DWORD colour;

	static const DWORD FORMAT = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	static const int STRIDE_SIZE = 16;
};
struct ColouredNormalVertex {
	float x, y, z;
	float nx, ny, nz;
	DWORD colour;

	static const DWORD FORMAT = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
	static const int STRIDE_SIZE = 28;
};

class Mesh : public Drawable3D {
	LPD3DXMESH              pMesh;                 // Our mesh object in sysmem
	D3DMATERIAL9*           pMeshMaterials; // Materials for our mesh
	LPDIRECT3DTEXTURE9*     pMeshTextures;  // Textures for our mesh
	DWORD                   dwNumMaterials = 0L;   // Number of mesh materials
public:
	Mesh();
	~Mesh();
	void initializeResources(LPDIRECT3DDEVICE9& device) override;
	void releaseResources() override;
	void draw(LPDIRECT3DDEVICE9& device) override;
};


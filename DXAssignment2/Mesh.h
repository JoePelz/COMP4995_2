#pragma once

#include <Windows.h>
#include <d3d9.h>
#include "Drawable3D.h"
#include "Errors.h"
#include "atlstr.h"

// Logically, we'll call our new untransformed vertex format UntransformedColouredVertex. An
// Untransformed vertex only holds a logical x, y, z point in space. It does not hold rhw.
struct UntransformedColouredVertex {
	float x, y, z;
	DWORD colour;

	// The format is ever so slightly different, and the stride size will be just 16 in
	// this format.
	static const DWORD FORMAT = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	static const int STRIDE_SIZE = 16;
};

// This is a Transformed vertex with a Diffuse colour component.
struct TransformedColouredVertex {
	float x, y, z, rhw;
	DWORD colour;

	static const DWORD FORMAT = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
	static const int STRIDE_SIZE = 20;
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


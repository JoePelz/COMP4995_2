#pragma once

#include <Windows.h>
#include <d3d9.h>
#include "Drawable3D.h"
#include "Errors.h"
#include "atlstr.h"

class Mesh : public Drawable3D {
	TCHAR* path;
	LPD3DXMESH              pMesh;          // Our mesh object in sysmem
	D3DMATERIAL9*           pMeshMaterials; // Materials for our mesh
	LPDIRECT3DTEXTURE9*     pMeshTextures;  // Textures for our mesh
	DWORD                   dwNumMaterials = 0L;   // Number of mesh materials
public:
	Mesh(TCHAR* filename);
	~Mesh();
	void initializeResources(LPDIRECT3DDEVICE9& device) override;
	void releaseResources() override;
	void draw(LPDIRECT3DDEVICE9& device) override;
};


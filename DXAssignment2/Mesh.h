#pragma once

#include <Windows.h>
#include <d3d9.h>
#include "Drawable3D.h"
#include "Errors.h"
#include "atlstr.h"

/*
A Mesh object is a 3D polygon mesh that can be rendered in 3D. The mesh must be loaded from a .x file.
*/

class Mesh : public Drawable3D {
	TCHAR* path; //The path to the .x file 
	LPD3DXMESH              pMesh;          // Our mesh object in sysmem
	D3DMATERIAL9*           pMeshMaterials; // Materials for our mesh
	LPDIRECT3DTEXTURE9*     pMeshTextures;  // Textures for our mesh
	DWORD                   dwNumMaterials = 0L;   // Number of mesh materials
public:
	Mesh(TCHAR* filename) : path{ filename } { }
	void initializeResources(LPDIRECT3DDEVICE9& device) override;
	void releaseResources() override;
	void draw(LPDIRECT3DDEVICE9& device, D3DXMATRIX* xform) override;
};


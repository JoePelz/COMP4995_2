#include "Mesh.h"


/*
Summary:
	Initialize the mesh within the Direct3D device.
Params: 
	device: The Direct3D device to install the mesh into.
Return: -
*/
void Mesh::initializeResources(LPDIRECT3DDEVICE9 & device) {
	LPD3DXBUFFER pD3DXMtrlBuffer;
	HRESULT r;
	// Load the mesh from the specified file
	r = D3DXLoadMeshFromX(path, D3DXMESH_SYSTEMMEM,
		device, NULL,
		&pD3DXMtrlBuffer, NULL, &dwNumMaterials,
		&pMesh);
	Errors::ErrorCheck(r, TEXT("Could not find %s"), path);

	// We need to extract the material properties and texture names from the 
	// pD3DXMtrlBuffer
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];
	pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

	for (DWORD i = 0; i<dwNumMaterials; i++) {
		// Copy the material
		pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;

		pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL && strlen(d3dxMaterials[i].pTextureFilename) > 0) {
			// Create the texture
			TCHAR texFileName[MAX_PATH];
			USES_CONVERSION;
			_tcscpy_s(texFileName, MAX_PATH, A2T(d3dxMaterials[i].pTextureFilename));

			r = D3DXCreateTextureFromFile(device, texFileName, &pMeshTextures[i]);
			Errors::ErrorCheck(r, TEXT("Could not find texture map for %s."), path);
		}
	}

	// Done with the material buffer
	pD3DXMtrlBuffer->Release();
}

/*
Summary:
	Uninstall the mesh from the Direct3D device and releases resources.
Params: -
Return: -
*/
void Mesh::releaseResources() {
	if (pMeshMaterials != NULL)
		delete[] pMeshMaterials;
	pMeshMaterials = NULL;

	if (pMeshTextures) {
		for (DWORD i = 0; i < dwNumMaterials; i++) {
			if (pMeshTextures[i])
				pMeshTextures[i]->Release();
		}
		delete[] pMeshTextures;
	}
	pMeshTextures = NULL;

	if (pMesh != NULL)
		pMesh->Release();
	pMesh = NULL;
}

/*
Summary:
	Triggers the mesh to be rendered on this frame.
Params: 
	device: the Direct3D device to render in.
Return: -
*/
void Mesh::draw(LPDIRECT3DDEVICE9 & device, D3DXMATRIX* xform) {
	if (xform) {
		device->SetTransform(D3DTS_WORLD, &getReflection(*xform));
	} else {
		device->SetTransform(D3DTS_WORLD, &getTransform());
	}

	for (DWORD i = 0; i<dwNumMaterials; i++) {
		// Set the material and texture for this subset
		device->SetMaterial(&pMeshMaterials[i]);
		device->SetTexture(0, pMeshTextures[i]);

		// Draw the mesh subset
		pMesh->DrawSubset(i);
	}
}

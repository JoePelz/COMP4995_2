#include "Mesh.h"



Mesh::Mesh() {
}


Mesh::~Mesh() {
}

void Mesh::initializeResources(LPDIRECT3DDEVICE9 & device) {
	LPD3DXBUFFER pD3DXMtrlBuffer;
	HRESULT r;
	// Load the mesh from the specified file
	r = D3DXLoadMeshFromX(TEXT("Tiger.x"), D3DXMESH_SYSTEMMEM,
		device, NULL,
		&pD3DXMtrlBuffer, NULL, &dwNumMaterials,
		&pMesh);
	if (FAILED(r)) 
	{
		// If model is not in current folder, try parent folder
		r = D3DXLoadMeshFromX(TEXT("..\\Tiger.x"), D3DXMESH_SYSTEMMEM,
			device, NULL,
			&pD3DXMtrlBuffer, NULL, &dwNumMaterials,
			&pMesh);
		Errors::ErrorCheck(r, TEXT("Could not find tiger.x"));
	}

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
			if (FAILED(r)) 
			{
				// If texture is not in current folder, try parent folder
				const TCHAR* strPrefix = TEXT("..\\");
				const int lenPrefix = lstrlen(strPrefix);
				TCHAR strTexture[MAX_PATH];
				lstrcpyn(strTexture, strPrefix, MAX_PATH);
				lstrcpyn(strTexture + lenPrefix, texFileName, MAX_PATH - lenPrefix);
				// If texture is not in current folder, try parent folder
				r = D3DXCreateTextureFromFile(device, strTexture, &pMeshTextures[i]);
				Errors::ErrorCheck(r, TEXT("Could not find tiger texture map."));
			}
		}
	}

	// Done with the material buffer
	pD3DXMtrlBuffer->Release();
}

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

void Mesh::draw(LPDIRECT3DDEVICE9 & device) {
	device->SetTransform(D3DTS_WORLD, &getTransform());

	for (DWORD i = 0; i<dwNumMaterials; i++) {
		// Set the material and texture for this subset
		device->SetMaterial(&pMeshMaterials[i]);
		device->SetTexture(0, pMeshTextures[i]);

		// Draw the mesh subset
		pMesh->DrawSubset(i);
	}
}

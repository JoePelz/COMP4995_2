#include "Cube.h"

/*
Summary:
	Creates a cube in device memory for rendering purposes.
Params: 
	device: The direct3D device to load a cube onto.
Return: -
*/
void Cube::initializeResources(LPDIRECT3DDEVICE9 & device) {
	
	ColouredNormalVertex vertices[] = {
		// Front Face (1-2-3-4)
		{ -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0xffff0000 },
		{ 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0xffffafaf },
		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0xffffafaf },
		{ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0xffff0000 },

		// Right Face (2-6-4-8)
		{ 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0xff00ff00 },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0xffafffaf },
		{ 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0xffafffaf },
		{ 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0xff00ff00 },

		// Top Face (5-6-1-2)
		{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0xff0000ff },
		{ 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0xffafafff },
		{ -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0xffafafff },
		{ 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0xff0000ff },

		// Back Face (6-5-8-7)
		{ 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0xffffff00 },
		{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0xffffffaf },
		{ 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0xffffffaf },
		{ -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0xffffff00 },

		// Left Face (5-1-7-3)
		{ -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0xffff00ff },
		{ -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0xffffafff },
		{ -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0xffffafff },
		{ -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0xffff00ff },

		// Bottom Face (3-4-7-8)
		{ -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0xff00ffff },
		{ 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0xffafffff },
		{ -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0xffafffff },
		{ 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0xff00ffff }
	};

	auto r = device->CreateVertexBuffer(sizeof(vertices), D3DUSAGE_WRITEONLY, ColouredNormalVertex_FLAGS, D3DPOOL_DEFAULT, &vertexBuffer_, NULL);
	Errors::ErrorCheck(r, TEXT("Initialize resources failed to create vertex buffer"));

	// Move vertices into the buffer:  Lock, Copy, Unlock
	void* bufferMemory;
	r = vertexBuffer_->Lock(0, sizeof(vertices), &bufferMemory, NULL);
	Errors::ErrorCheck(r, TEXT("Initialize resources failed to get a vertexBuffer lock"));
	memcpy(bufferMemory, vertices, sizeof(vertices));
	vertexBuffer_->Unlock();
}

/*
Summary:
	Releases this asset's device resources.
Params: -
Return: -
*/
void Cube::releaseResources() {
	if (vertexBuffer_) {
		vertexBuffer_->Release();
		vertexBuffer_ = NULL;
	}
}

/*
Summary:
	Sends the cube to be rendered at it's current transformed location.
Params:
	device: The direct3D device to draw in.
Return: -
*/
void Cube::draw(LPDIRECT3DDEVICE9 & device) {
	device->SetFVF(ColouredNormalVertex_FLAGS);
	device->SetStreamSource(0, vertexBuffer_, 0, ColouredNormalVertex_STRIDE);

	device->SetRenderState(D3DRS_COLORVERTEX, true);

	device->SetTransform(D3DTS_WORLD, &getTransform());
	for (int i = 0; i < 6; i++) {
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}

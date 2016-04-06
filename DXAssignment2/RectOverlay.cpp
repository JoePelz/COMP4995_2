#include "RectOverlay.h"

void RectOverlay::initializeResources(LPDIRECT3DDEVICE9 & device) {
	TexturedVertex vertices[] = {
		{  1.0f,  1.0f, -1.0f, 0.0f, 0.0f },
		{ -1.0f,  1.0f, -1.0f, 1.0f, 0.0f },
		{  1.0f, -1.0f, -1.0f, 0.0f, 1.0f },
		{ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f }
	};

	auto r = device->CreateVertexBuffer(sizeof(vertices), D3DUSAGE_WRITEONLY, TexturedVertex_FLAGS, D3DPOOL_DEFAULT, &vertexBuffer_, NULL);
	Errors::ErrorCheck(r, TEXT("Initialize resources failed to create vertex buffer"));

	// Move vertices into the buffer:  Lock, Copy, Unlock
	void* bufferMemory;
	r = vertexBuffer_->Lock(0, sizeof(vertices), &bufferMemory, NULL);
	Errors::ErrorCheck(r, TEXT("Initialize resources failed to get a vertexBuffer lock"));
	memcpy(bufferMemory, vertices, sizeof(vertices));
	vertexBuffer_->Unlock();
}

void RectOverlay::releaseResources() {
	if (vertexBuffer_) {
		vertexBuffer_->Release();
		vertexBuffer_ = NULL;
	}
}

void RectOverlay::setupMaterial() {
	mat_ = D3DMATERIAL9();

	// Set the RGBA for diffuse reflection.
	mat_.Diffuse.r = 1.0f;
	mat_.Diffuse.g = 1.0f;
	mat_.Diffuse.b = 1.0f;
	mat_.Diffuse.a = 1.0f;

	// Set the RGBA for ambient reflection.
	mat_.Ambient.r = 1.0f;
	mat_.Ambient.g = 1.0f;
	mat_.Ambient.b = 1.0f;
	mat_.Ambient.a = 1.0f;

	// Set the color and sharpness of specular highlights.
	mat_.Specular.r = 1.0f;
	mat_.Specular.g = 1.0f;
	mat_.Specular.b = 1.0f;
	mat_.Specular.a = 1.0f;
	mat_.Power = 2.0f;

	// Set the RGBA for emissive color.
	mat_.Emissive.r = 0.0f;
	mat_.Emissive.g = 0.0f;
	mat_.Emissive.b = 0.0f;
	mat_.Emissive.a = 0.0f;

}

void RectOverlay::draw(LPDIRECT3DDEVICE9 & device, const D3DXMATRIX * xform) {
	device->SetFVF(TexturedVertex_FLAGS);
	device->SetStreamSource(0, vertexBuffer_, 0, TexturedVertex_STRIDE);

	device->SetTransform(D3DTS_WORLD, &getTransform());
	device->SetMaterial(&mat_);
	device->SetTexture(0, tex_);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void RectOverlay::drawLite(LPDIRECT3DDEVICE9 & device, const D3DXMATRIX * xform) {
	device->SetFVF(TexturedVertex_FLAGS);
	device->SetStreamSource(0, vertexBuffer_, 0, TexturedVertex_STRIDE);

	device->SetTransform(D3DTS_WORLD, &getTransform());
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

RectOverlay::RectOverlay() {
	setPosition({ 0, 0, -1 });
	setScale({ 1.0f, 1.0f, 1.0f });
	setupMaterial();
}


RectOverlay::~RectOverlay() {
	releaseResources();
}

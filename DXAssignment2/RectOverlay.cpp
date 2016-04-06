#include "RectOverlay.h"


/*
List of shaders available to cycle through.
*/
const TCHAR * const RectOverlay::shaders_[] = {
	TEXT("shaderNone.fx"),
	TEXT("shaderSepia.fx"),
	TEXT("shaderGray.fx"),
	TEXT("shaderInvert.fx")
};

/*
Summary:
	Initialize the vertices, and create a vertex declaration for the shader.
Params: 
	device: the device to register and allocate within.
Return: -
*/
void RectOverlay::initVertices(IDirect3DDevice9 * device) {
	TexturedVertex vertices[] = {
		{ 1.0f,  1.0f, -1.0f, 0.0f, 0.0f },
		{ -1.0f,  1.0f, -1.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, -1.0f, 0.0f, 1.0f },
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

	//Init All Vertex Declarations
	D3DVERTEXELEMENT9 VertexPNTElements[] =
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	r = device->CreateVertexDeclaration(VertexPNTElements, &vertDecl_);
	Errors::ErrorCheck(r, TEXT("Vertex Declaration failed"));
}

/*
Summary:
	Initialize the custom texture.
Params: 
	device: the device to register and allocate within.
Return: -
*/
void RectOverlay::initTexture(IDirect3DDevice9 * device) {
	LPDIRECT3DSURFACE9 backBuffer;
	D3DSURFACE_DESC description;

	HRESULT r = device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	if (FAILED(r) || FAILED(backBuffer->GetDesc(&description))) {
		Errors::SetError(TEXT("Couldn't obtain backBuffer information"));
		return;
	}
	backBuffer->Release();
	backBuffer = 0;

	device->CreateTexture(
		description.Width, 
		description.Height, 
		1, 
		D3DUSAGE_RENDERTARGET, 
		description.Format, 
		D3DPOOL_DEFAULT, 
		&tex_, 
		NULL);

	r = tex_->GetSurfaceLevel(0, &texSurface_);
	Errors::ErrorCheck(r, TEXT("Error getting surface level 0"));
}

/*
Summary:
	Initialize the new pixel shader.
Params: 
	device: the device to register and allocate within.
Return: -
*/
void RectOverlay::initPixelShader(IDirect3DDevice9 * device) {
	//Set up pixel shader mumbo jumbo
	ID3DXBuffer* errors = 0;
	D3DXCreateEffectFromFile(device, shaders_[shaderIndex_], 0, 0, D3DXSHADER_DEBUG, 0, &shader_, &errors);
	if (errors)
		MessageBox(0, (TCHAR*)errors->GetBufferPointer(), 0, 0);

	// Obtain handles.
	shTechnique_ = shader_->GetTechniqueByName("TransformTech");
	shWorld_ = shader_->GetParameterByName(0, "gWorld");
	shTex_ = shader_->GetParameterByName(0, "gTex");
}

/*
Summary:
	Initialize the resources used by the overlay within the Direct3D device context.
Params: 
	device: the device to register and allocate within.
Return: -
*/
void RectOverlay::initializeResources(LPDIRECT3DDEVICE9 & device) {
	initVertices(device);
	initTexture(device);
	initPixelShader(device);
}

/*
Summary:
	Release all the resources used by the overlay.
Params: -
Return: -
*/
void RectOverlay::releaseResources() {
	if (vertexBuffer_) {
		vertexBuffer_->Release();
		vertexBuffer_ = NULL;
	}
	if (texSurface_ != NULL) {
		texSurface_->Release();
		texSurface_ = NULL;
	}
	if (tex_ != NULL) {
		tex_->Release();
		tex_ = NULL;
	}
	if (shader_) {
		shader_->Release();
		shader_ = NULL;
	}
	if (vertDecl_) {
		vertDecl_->Release();
		vertDecl_ = NULL;
	}
}

/*
Summary:
	Render the overlay.
Params: 
	device: the device to draw in.
	xform: any additional transformations to append
Return: -
*/
void RectOverlay::draw(LPDIRECT3DDEVICE9 & device, const D3DXMATRIX* xform) {
	device->SetVertexDeclaration(vertDecl_);
	HRESULT r = shader_->SetTechnique(shTechnique_);
	Errors::ErrorCheck(r, TEXT("FX setTechnique failed"));

	UINT numPasses = 0;
	r = shader_->Begin(&numPasses, 0);
	Errors::ErrorCheck(r, TEXT("FX Begin failed"));

	//device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

	device->SetFVF(TexturedVertex_FLAGS);
	device->SetStreamSource(0, vertexBuffer_, 0, TexturedVertex_STRIDE);

	for (UINT i = 0; i < numPasses; ++i) {
		r = shader_->BeginPass(i);
		Errors::ErrorCheck(r, TEXT("FX Begin Pass failed"));
		r = shader_->SetTexture(shTex_, tex_);
		Errors::ErrorCheck(r, TEXT("FX Set Texture failed"));
		r = shader_->SetMatrix(shWorld_, &getTransform());
		Errors::ErrorCheck(r, TEXT("FX Set Matrix failed"));
		r = shader_->CommitChanges();
		Errors::ErrorCheck(r, TEXT("FX Commit Changes failed"));
		//----------------------
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		//----------------------
		r = shader_->EndPass();
		Errors::ErrorCheck(r, TEXT("FX End Pass failed"));
	}
	r = shader_->End();
	Errors::ErrorCheck(r, TEXT("FX End failed"));

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

/*
Summary:
	Update the texture by copying in from the backBuffer
Params: 
	device: the device to work within.
	backBuffer: the current back buffer to copy the texture from.
Return: -
*/
void RectOverlay::updateTexture(IDirect3DDevice9* device, IDirect3DSurface9 * backBuffer) {
	HRESULT r;
	r = device->StretchRect(backBuffer, NULL, texSurface_, NULL, D3DTEXF_LINEAR);
	Errors::ErrorCheck(r, TEXT("Error over StretchRect"));
}

/*
Summary:
	Cycle through the available shaders. Releases and reinitializes the overlay.
Params: 
	device: the Direct3D device context to work in.
Return: -
*/
void RectOverlay::cycleShader(IDirect3DDevice9* device) {
	releaseResources();
	//change shader assignment
	shaderIndex_ = (shaderIndex_ + 1) % 4;
	initializeResources(device);
}

/*
Summary:
	Constructor: create an overlay and put it directly in front of the camera.
Params: 
	device: the device to draw in.
	xform: any additional transformations to append
Return: -
*/
RectOverlay::RectOverlay() {
	setPosition({ 0, 0, -1 });
	shaderIndex_ = 0;
}


RectOverlay::~RectOverlay() {
	releaseResources();
}

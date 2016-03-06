#include "Light.h"

//static member, to track light array index within the Direct3D device.
int Light::si_light = 0;

/*
Summary:
	Constructor: Create a default light of the specified type.
Params: 
	type: the light type to use. 
Return: -
*/
Light::Light(D3DLIGHTTYPE type) {
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = type;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Ambient.r = 0.0f;
	light.Ambient.g = 0.0f;
	light.Ambient.b = 0.0f;
	light.Specular.r = 1.0f;
	light.Specular.g = 1.0f;
	light.Specular.b = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Range = 3000.0f;
	light.Falloff = 1.0f;
	light.Phi = 0.32f * D3DX_PI;
	light.Theta = 0.3f * D3DX_PI;
}

/*
Summary:
	Initialize the light within Direct3D
Params: 
	device: The Direct3D to install the light into.
Return: -
*/
void Light::initializeResources(LPDIRECT3DDEVICE9 & device) {
	index = si_light++;
	device->SetLight(index, &light);
	device->LightEnable(index, true);
}

/*
Summary:
	Release the light from the Direct3D Device.
Params: -
Return: -
*/
void Light::releaseResources() {
	--si_light;
}

#include "Controller.h"

Controller::Controller(HINSTANCE hInstance)
	: hInstance(hInstance) {
	Cube* c = new Cube();
	pDrawable3D myObj(c);
	gameModel.add3D(myObj);

	c = new Cube();
	c->setPosition({ 0.0f, -0.5f, 0.0f });
	c->setScale({ 10.0f, 1.0f, 10.0f });
	pDrawable3D myObj2(c);
	gameModel.add3D(myObj2);

	Mesh* m = new Mesh(TEXT("Viggen.x"));
	m->setPosition({ 2.0f, 0.5f, 0.0f });
	m->setScale({ 2.0f, 2.0f, 2.0f });
	pDrawable3D myMesh(m);
	gameModel.add3D(myMesh);

	m = new Mesh(TEXT("Viggen.x"));
	m->setPosition({ -2.0f, 0.5f, 0.0f });
	m->setScale({ 2.0f, 2.0f, 2.0f });
	pDrawable3D myMesh2(m);
	gameModel.add3D(myMesh2);

	pLight myLight1(new Light(D3DLIGHT_DIRECTIONAL));
	myLight1->setPosition(0, 5, 0);
	gameModel.addLight(myLight1);

}

Controller::~Controller() {
	GameShutdown();
}

void Controller::setHWnd(const HWND newHWnd) {
	hWnd = newHWnd;
}

//Static
long CALLBACK Controller::windowLoop(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	LPCREATESTRUCT cs;
	static Controller* ctrl;

	switch (uMessage) {
	case WM_CREATE:
	{
		//save the pointer to the controller class
		cs = (LPCREATESTRUCT)lParam;
		if ((LONG)cs->lpCreateParams) {
			SetClassLongPtr(hWnd, 0, (LONG)cs->lpCreateParams);
		}

		ctrl = (Controller*)GetClassLongPtr(hWnd, 0);
		//ctrl->setHWnd(hWnd);
		
		return 0;
	}
	case WM_PAINT:
		ValidateRect(hWnd, NULL);//basically saying - yeah we took care of any paint msg without any overhead
		return 0;
	case WM_LBUTTONDOWN:
		ctrl->MouseDown(lParam, 0);
		return 0;
	case WM_MBUTTONDOWN:
		ctrl->MouseDown(lParam, 1);
		return 0;
	case WM_RBUTTONDOWN:
		ctrl->MouseDown(lParam, 2);
		return 0;
	case WM_MOUSEMOVE:
		ctrl->MouseMove(lParam);
		return 0;
	case WM_LBUTTONUP:
		ctrl->MouseUp(lParam, 0);
		return 0;
	case WM_MBUTTONUP:
		ctrl->MouseUp(lParam, 1);
		return 0;
	case WM_RBUTTONUP:
		ctrl->MouseUp(lParam, 2);
		return 0;
	case WM_KEYDOWN:
		if (ctrl->KeyDown(wParam))
			return 0;
		break;
	case WM_KEYUP:
		if (ctrl->KeyUp(wParam))
			return 0;
		break;
	case WM_MOUSEWHEEL:
		ctrl->MouseWheel(lParam, wParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

void Controller::MouseDown(LPARAM lParam, int btn) {
	input.mpos.x = GET_X_LPARAM(lParam);
	input.mpos.y = GET_Y_LPARAM(lParam);
	if (btn == 0)
		input.lbutton = true;
	else if (btn == 1)
		input.mbutton = true;
	else if (btn == 2)
		input.rbutton = true;
	//Errors::SetError(TEXT("Mouse down at (%d, %d)"), xPos, yPos);
}

/*
Summary: MouseMove captures mouse movement events that occurs but only while the left mouse button is down.
Params: lParam holds the coordinates of the mouse in client window space. Access values via GET_X_LPARAM(lParam) and GET_Y_LPARAM(lParam) macros.
Return: -
*/
void Controller::MouseMove(LPARAM lParam) {
	//if no mouse buttons, don't bother
	if (!input.lbutton && !input.rbutton && !input.mbutton) return;

	input.mdelta.x = (float)(GET_X_LPARAM(lParam) - input.mpos.x) / (float)gameModel.getWidth();
	input.mdelta.y = (float)(GET_Y_LPARAM(lParam) - input.mpos.y) / (float)gameModel.getHeight();

	input.mpos.x = GET_X_LPARAM(lParam);
	input.mpos.y = GET_Y_LPARAM(lParam);
}

void Controller::MouseUp(LPARAM lParam, int btn) {
	if (btn == 0)
		input.lbutton = false;
	else if (btn == 1)
		input.mbutton = false;
	else if (btn == 2)
		input.rbutton = false;
}

bool Controller::KeyDown(WPARAM wParam) {
	switch (wParam) {
	case VK_F:
		try {
			if (gameModel.getFullscreen()) {
				//Go window mode
				gameModel.setDisplayMode(640, 480, FALSE);
				SetWindowPos(hWnd, HWND_NOTOPMOST, NULL, NULL, gameModel.getWidth(), gameModel.getHeight(), SWP_NOACTIVATE | SWP_NOMOVE);
			} else {
				//Go fullscreen
				gameModel.setDisplayMode(800, 600, TRUE);
			}
		} catch (LPTSTR error) {
			_tprintf_s(TEXT("%s"), error);
			Abort(1);
		}
		releaseResources();
		renderEngine.ChangeDisplayMode(gameModel);
		initializeResources();
		return TRUE;
	case VK_W:
		return input.key_w = true;
	case VK_S:
		return input.key_s = true;
	case VK_A:
		return input.key_a = true;
	case VK_D:
		return input.key_d = true;
	case VK_E:
		return input.key_e = true;
	case VK_C:
		return input.key_c = true;
	case VK_0:
		gameModel.setSelection(&gameModel.getCamera());
		return true;
	case VK_1:
		gameModel.setSelection(gameModel.get3D()[0].get());
		return true;
	case VK_2:
		gameModel.setSelection(gameModel.get3D()[1].get());
		return true;
	case VK_3:
		gameModel.setSelection(gameModel.get3D()[2].get());
		return true;
	case VK_4:
		gameModel.setSelection(gameModel.get3D()[3].get());
		return true;
	}

	return false;
}

bool Controller::KeyUp(WPARAM wParam) {
	switch (wParam) {
	case VK_W:
		return input.key_w = false;
	case VK_S:
		return input.key_s = false;
	case VK_A:
		return input.key_a = false;
	case VK_D:
		return input.key_d = false;
	case VK_E:
		return input.key_e = false;
	case VK_C:
		return input.key_c = false;
	}
	return false;
}

void Controller::MouseWheel(LPARAM pos, WPARAM scroll) {
	input.scrollAmount = GET_WHEEL_DELTA_WPARAM(scroll) / 120;
}

void Controller::GameStartup() {
	renderEngine.startEngine(hWnd, gameModel);
	initializeResources();
}

void Controller::initializeResources() {
	auto device = renderEngine.getDevice();

	for (auto& obj : gameModel.get3D()) {
		obj->initializeResources(device);
	}
	/*
	for (auto& obj : gameModel.getLights()) {
		obj->initializeResources(device);
	}
	*/

	D3DLIGHT9 d3dLight;
	HRESULT   hr;

	// Initialize the structure.
	ZeroMemory(&d3dLight, sizeof(d3dLight));

	// Set up a white point light.
	d3dLight.Type = D3DLIGHT_DIRECTIONAL;
	d3dLight.Diffuse.r = 1.0f;
	d3dLight.Diffuse.g = 1.0f;
	d3dLight.Diffuse.b = 1.0f;
	d3dLight.Ambient.r = 0.3f;
	d3dLight.Ambient.g = 0.3f;
	d3dLight.Ambient.b = 0.3f;
	d3dLight.Specular.r = 1.0f;
	d3dLight.Specular.g = 1.0f;
	d3dLight.Specular.b = 1.0f;

	d3dLight.Position.x = 0.0f;
	d3dLight.Position.y = 1.5f;
	d3dLight.Position.z = 0.0f;

	d3dLight.Direction.x = 0.0f;
	d3dLight.Direction.y = -1.0f;
	d3dLight.Direction.z = 0.0f;

	// Don't attenuate.
	d3dLight.Attenuation0 = 1.0f;
	d3dLight.Range = 2000.0f;

	// Set the property information for the first light.
	hr = device->SetLight(0, &d3dLight);
	if (SUCCEEDED(hr))
		// Handle Success
		OutputDebugString(TEXT("light succeeded\n"));
	else
		// Handle failure
		OutputDebugString(TEXT("light failed\n"));

	hr = device->LightEnable(0, true);
	if (SUCCEEDED(hr))
		// Handle Success
		OutputDebugString(TEXT("light enable succeeded\n"));
	else
		// Handle failure
		OutputDebugString(TEXT("light enable failed\n"));

	
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	//device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(128, 128, 128));


	//Initialize frame counter
	gameModel.initFrameTimer();
	FrameRate* tw = new FrameRate(renderEngine.getDevice(), TEXT("font.bmp"), 10, 12, &gameModel);
	tw->setPosition(10, 10);
	tw->setTransparentColor(D3DCOLOR_ARGB(0, 255, 0, 255));
	std::shared_ptr<Drawable2D> drawableText(tw);
	gameModel.addFG(drawableText);


	//Initialize the background image
	Background* bg = new Background(renderEngine.getDevice());
	bg->setImage(TEXT(DEFAULT_BITMAP));
	std::shared_ptr<Drawable2D> drawable(bg);
	gameModel.addBG(drawable);
}

void Controller::releaseResources() {
	gameModel.clearBG();
	gameModel.clearFG();

	for (auto& obj : gameModel.get3D()) {
		obj->releaseResources();
	}
}

void Controller::GameLoop() {
	updateModel(input, gameModel);
	renderEngine.render(gameModel);
	gameModel.setFrameTick();

	if (GetAsyncKeyState(VK_ESCAPE)) {
		PostQuitMessage(0);
	}
}

void Controller::updateModel(Input& input, Model& model) {
	ITransform* sel = model.getSelection();
	float ms = model.getFrameTime();
	Camera& cam = model.getCamera();

	float factor = ms * SENSITIVITY * 0.0001f;

	if (input.key_w) {
		sel->translate(cam.getDirection() * factor);
	}
	if (input.key_s) {
		sel->translate(-cam.getDirection() * factor);
	}
	if (input.key_a) {
		sel->translate(cam.getRight() * factor);
	}
	if (input.key_d) {
		sel->translate(-cam.getRight() * factor);
	}
	if (input.key_e) {
		sel->translate(cam.getUp() * factor);
	}
	if (input.key_c) {
		sel->translate(-cam.getUp() * factor);
	}

	if (input.lbutton) {
		if (sel == &cam) {
			cam.addRotation(input.mdelta);
		} else {
			float angle = (input.mdelta.x * input.mdelta.x + input.mdelta.y * input.mdelta.y) * SENSITIVITY;
			if (angle != 0) {
				D3DXVECTOR3 axis;
				D3DXVECTOR3 dir = cam.getUp() * input.mdelta.y + cam.getRight() * input.mdelta.x;
				D3DXVec3Cross(&axis, &cam.getDirection(), &dir);
				sel->rotate(axis, angle * 3);
			}
		}
		input.mdelta.x = 0;
		input.mdelta.y = 0;
	}

	if (input.scrollAmount != 0) {
		if (sel == &cam) {
			cam.setFOV(cam.getFOV() * ((float)input.scrollAmount * -0.2f + 1.0f));
		} else {
			sel->scale(input.scrollAmount * 0.1f + 1.0f);
		}
		input.scrollAmount = 0;
	}
}

void Controller::GameShutdown() {
	releaseResources();
	renderEngine.stopEngine();
}

void Controller::Abort(int errorCode) {
	GameShutdown();
	PostMessage(hWnd, WM_DESTROY, errorCode, errorCode);
}

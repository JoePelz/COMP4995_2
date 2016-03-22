#include "Controller.h"

/*
Summary:
	Constructor. creates all asset objects, hard-coded, and adds them to the appropriate collections.
Params: hInstance: the current instance of the running program.
Return: -
*/
Controller::Controller(HINSTANCE hInstance)
	: hInstance(hInstance) {

	//============================================================
    //                        2D Elements
	//============================================================
	FrameRate* fr = new FrameRate(TEXT("font.bmp"), 10, 12, &gameModel);
	gameModel.initFrameTimer();
	fr->setPosition(10, 10);
	fr->setTransparentColor(D3DCOLOR_ARGB(0, 255, 0, 255));
	pDrawable2D drawableText(fr);
	gameModel.addFG(drawableText);

	//Initialize the background image
	Background* bg = new Background(TEXT(DEFAULT_BITMAP));
	std::shared_ptr<Drawable2D> drawable(bg);
	gameModel.addBG(drawable);

	//============================================================
	//                        3D Geometry
	//============================================================
	//Viggen retrieved from http://www.sandbox.de/osg/ 
	Mesh* m = new Mesh(TEXT("Viggen.x"));
	m->setPosition({ 2.0f, 0.5f, 0.0f });
	m->setScale({ 2.0f, 2.0f, 2.0f });
	pDrawable3D myMesh(m);
	gameModel.add3D(myMesh);

	//Harrier jet, also from http://www.sandbox.de/osg/ 
	m = new Mesh(TEXT("Harrier.x"));
	m->setPosition({ 0.0f, 1.5f, 0.0f });
	m->rotate({ 0, 1, 0 }, D3DX_PI / 2);
	pDrawable3D myMesh1(m);
	gameModel.add3D(myMesh1);

	//Mirror Cube
	MirrorCube* mc = new MirrorCube();
	mc->setPosition({ 0.0f, 0.0f, -2.0f });
	mc->setScale({ 1.0f, 1.0f, 1.0f });
	std::shared_ptr<MirrorCube> myMC(mc);
	gameModel.setMirror(myMC);

	//little center cube
	Cube* c = new Cube();
	pDrawable3D myObj(c);
	gameModel.add3D(myObj);

	//Big floor cube
	c = new Cube();
	c->setPosition({ 0.0f, -50, 0.0f });
	c->setScale({ 10.0f, 50.0f, 10.0f });
	pDrawable3D myObj2(c);
	gameModel.add3D(myObj2);

	//============================================================
	//                        Lights
	//============================================================
	pLight myLight1(new Light(D3DLIGHT_DIRECTIONAL));
	myLight1->setDirection({ 1.0f, -0.5f, 0.0f });
	gameModel.addLight(myLight1);

	pLight myLight2(new Light(D3DLIGHT_POINT));
	myLight2->setPosition(0, 2, 0);
	gameModel.addLight(myLight2);

	pLight myLight3(new Light(D3DLIGHT_SPOT));
	myLight3->setPosition(2.0f, 5.0f, 0.0f);
	myLight3->setDirection({ 0.0f, -1.0f, 0.0f });
	gameModel.addLight(myLight3);
}


/*
Summary:
	Destructor. Shuts down the 3d engine.
Params: -
Return: -
*/
Controller::~Controller() {
	GameShutdown();
}

/*
Summary:
	Set the handle to the window this controller is meant to work within.
Params: 
	newHWnd the handle to the window for this controller to work within.
Return: -
*/
void Controller::setHWnd(const HWND newHWnd) {
	hWnd = newHWnd;
}

/*
Summary: static function to act as WNDPROC handling messages from the win32 system. Forwards input messages to Controller:: member functions.
Params: 
	hWnd: Handle to the window the message is for.
	uMessage: Numeric message id identifying what happened.
	wParam: additional message-specific data
	lParam: additional message-specific data
Return: 0 if message is handled. 
*/
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


/*
Summary:
	Callback for mouse button presses. Updates input state object.
Params:
	lParam: the x and y coordinates of the mouse at time of button pressing. Access members by GET_X_LPARAM and GET_Y_LPARAM macros.
	btn: The button that was pressed where 0 is the left click, 1 is middle click, 2 is right click.
Return: -
*/
void Controller::MouseDown(LPARAM lParam, int btn) {
	input.mpos.x = GET_X_LPARAM(lParam);
	input.mpos.y = GET_Y_LPARAM(lParam);
	if (btn == 0)
		input.lbutton = true;
	else if (btn == 1)
		input.mbutton = true;
	else if (btn == 2)
		input.rbutton = true;
}

/*
Summary: 
	Callback for mouse motion. Updates input state object
Params: 
	lParam holds the coordinates of the mouse in client window space. Access values via GET_X_LPARAM(lParam) and GET_Y_LPARAM(lParam) macros.
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

/*
Summary:
	Callback for mouse button release. Updates input state object.
Params:
	lParam holds the coordinates of the mouse in client window space. Access values via GET_X_LPARAM(lParam) and GET_Y_LPARAM(lParam) macros.
	btn: The button that was pressed where 0 is the left click, 1 is middle click, 2 is right click.
Return: -
*/
void Controller::MouseUp(LPARAM lParam, int btn) {
	if (btn == 0)
		input.lbutton = false;
	else if (btn == 1)
		input.mbutton = false;
	else if (btn == 2)
		input.rbutton = false;
}

/*
Summary:
	Callback for keypress events. Updates input state object.
Params:
	wParam holds which key was pressed. Compare with VK_* and see defines.h for some values.
Return: true if the keypress is handled
*/
bool Controller::KeyDown(WPARAM wParam) {
	DWORD temp;
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
		gameModel.setSelection(gameModel.getMirror().get());
		return true;
	case VK_U: //toggle ambient light
		renderEngine.getDevice()->GetRenderState(D3DRS_AMBIENT, &temp);
		if (temp == D3DCOLOR_XRGB(128, 128, 128)) {
			renderEngine.getDevice()->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(0, 0, 0));
		} else {
			renderEngine.getDevice()->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(128, 128, 128));
		}
		return true;
	case VK_I: //toggle directional light
		renderEngine.getDevice()->GetLightEnable(0, (BOOL*)&temp);
		renderEngine.getDevice()->LightEnable(0, !temp);
		return true;
	case VK_O: //toggle point light
		renderEngine.getDevice()->GetLightEnable(1, (BOOL*)&temp);
		renderEngine.getDevice()->LightEnable(1, !temp);
		return true;
	case VK_P: //toggle spot light
		renderEngine.getDevice()->GetLightEnable(2, (BOOL*)&temp);
		renderEngine.getDevice()->LightEnable(2, !temp);
		return true;

	}

	return false;
}

/*
Summary:
	Callback for key release events. Updates input state object.
Params:
	wParam holds which key was pressed. Compare with VK_* and see defines.h for some values.
Return: true if the keypress is handled.
*/
bool Controller::KeyUp(WPARAM wParam) {
	switch (wParam) {
	case VK_W:
		input.key_w = false;
		return true;
	case VK_S:
		input.key_s = false;
		return true;
	case VK_A:
		input.key_a = false;
		return true;
	case VK_D:
		input.key_d = false;
		return true;
	case VK_E:
		input.key_e = false;
		return true;
	case VK_C:
		input.key_c = false;
		return true;
	}
	return false;
}

/*
Summary:
	Callback for mouse wheel events. Also fires on finger zooming. Updates input state object.
Params:
	pos: holds the coordinates of the mouse in client window space. Access values via GET_X_LPARAM(lParam) and GET_Y_LPARAM(lParam) macros.
	scroll: holds 120 * the number of mouse-wheel clicks that were scrolled. Access value with GET_WHEEL_DELTA_WPARAM(scroll) macro.
Return: -
*/
void Controller::MouseWheel(LPARAM pos, WPARAM scroll) {
	input.scrollAmount = GET_WHEEL_DELTA_WPARAM(scroll) / 120;
}

/*
Summary:
	Perform game engine and asset initialization.  Will throw exception if engine cannot be initialized.
Params: -
Return: -
*/
void Controller::GameStartup() {
	renderEngine.startEngine(hWnd, gameModel);
	initializeResources();
}

/*
Summary:
	Initialize (e.g. allocate) the resources for all assets within the Direct3D Device and set the render state
Params: -
Return: -
*/
void Controller::initializeResources() {
	auto device = renderEngine.getDevice();

	for (auto& obj : gameModel.get3D()) {
		obj->initializeResources(device);
	}
	gameModel.getMirror()->initializeResources(device);
	for (auto& obj : gameModel.getLights()) {
		obj->initializeResources(device);
	}
	for (auto& obj : gameModel.getBG()) {
		obj->initializeResources(device);
	}
	for (auto& obj : gameModel.getFG()) {
		obj->initializeResources(device);
	}

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(128, 128, 128));
}

/*
Summary:
	Release and free all game resources so the engine can be reset or shut down safely.
Params: -
Return: -
*/
void Controller::releaseResources() {
	for (auto& obj : gameModel.get3D()) {
		obj->releaseResources();
	}
	gameModel.getMirror()->releaseResources();
	for (auto& obj : gameModel.getLights()) {
		obj->releaseResources();
	}
	for (auto& obj : gameModel.getBG()) {
		obj->releaseResources();
	}
	for (auto& obj : gameModel.getFG()) {
		obj->releaseResources();
	}
}

/*
Summary:
	Main game loop. Updates the model, renders the frame, updates the frame timer, and checks for the escape key (to quit).
Params: -
Return: -
*/
void Controller::GameLoop() {
	updateModel(input, gameModel);
	renderEngine.render(gameModel);
	gameModel.setFrameTick();

	if (GetAsyncKeyState(VK_ESCAPE)) {
		PostQuitMessage(0);
	}
}

/*
Summary:
	Performs any game state updates based on the current state of the input for this frame, and the amount of time elapsed.
Params:
	input: The current state of the input devices (keyboard, mouse)
	model: the current state of the game (e.g. transformation matrices, lights on/off)
Return: -
*/
void Controller::updateModel(Input& input, Model& model) {
	Transform3D* sel = model.getSelection();
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

/*
Summary:
	Shuts down the engine.  Releases hardware and video resources, then shutdown down the engine.
Params: -
Return: -
*/
void Controller::GameShutdown() {
	releaseResources();
	renderEngine.stopEngine();
}

/*
Summary:
	Exits the program as soon as possible. 
Params:
	errorCode: The code to exit with.
Return: -
*/
void Controller::Abort(int errorCode) {
	GameShutdown();
	PostMessage(hWnd, WM_DESTROY, errorCode, errorCode);
}

#include "Controller.h"

Controller::Controller(HINSTANCE hInstance)
	: hInstance(hInstance) {
	Cube* c = new Cube();
	std::shared_ptr<Drawable3D> myObj(c);
	gameModel.add3D(myObj);
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
		ctrl->MouseDown(lParam);
		return 0;
	case WM_MOUSEMOVE:
		ctrl->MouseMove(lParam);
		return 0;
	case WM_LBUTTONUP:
		ctrl->MouseUp(lParam);
		return 0;
	case WM_KEYDOWN:
		if (ctrl->KeyDown(wParam)) {
			return 0;
		}
		break;
	case WM_KEYUP:
		if (ctrl->KeyUp(wParam)) {
			return 0;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

void Controller::MouseDown(LPARAM lParam) {
	input.mpos.x = GET_X_LPARAM(lParam);
	input.mpos.y = GET_Y_LPARAM(lParam);
	input.lbutton = true;
	//Errors::SetError(TEXT("Mouse down at (%d, %d)"), xPos, yPos);
}

/*
Summary: MouseMove captures mouse movement events that occurs but only while the left mouse button is down.
Params: lParam holds the coordinates of the mouse in client window space. Access values via GET_X_LPARAM(lParam) and GET_Y_LPARAM(lParam) macros.
Return: -
*/
void Controller::MouseMove(LPARAM lParam) {
	if (!input.lbutton) return;

	POINTFLOAT delta;
	delta.x = (float)(GET_X_LPARAM(lParam) - input.mpos.x) / (float)gameModel.getWidth();
	delta.y = (float)(GET_Y_LPARAM(lParam) - input.mpos.y) / (float)gameModel.getHeight();

	input.mpos.x = GET_X_LPARAM(lParam);
	input.mpos.y = GET_Y_LPARAM(lParam);

	gameModel.getCamera().addRotation(delta);
}

void Controller::MouseUp(LPARAM lParam) {
	input.lbutton = false;
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
	}

	return FALSE;
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

void Controller::GameStartup() {
	renderEngine.startEngine(hWnd, gameModel);
	initializeResources();
}

void Controller::initializeResources() {
	HRESULT r;
	auto device = renderEngine.getDevice();

	for (auto& obj : gameModel.get3D()) {
		obj->initializeResources(device);
	}
	
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);


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

	if (vertexBuffer_) {
		vertexBuffer_->Release();
		vertexBuffer_ = NULL;
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

void Controller::updateModel(const Input& input, Model& model) {
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
}

void Controller::GameShutdown() {
	releaseResources();
	renderEngine.stopEngine();
}

void Controller::Abort(int errorCode) {
	GameShutdown();
	PostMessage(hWnd, WM_DESTROY, errorCode, errorCode);
}

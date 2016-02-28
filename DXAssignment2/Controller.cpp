#include "Controller.h"

Controller::Controller(HINSTANCE hInstance)
	: hInstance(hInstance) {
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
	{
		ValidateRect(hWnd, NULL);//basically saying - yeah we took care of any paint msg without any overhead
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		ctrl->MouseDown(lParam);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		ctrl->MouseMove(lParam);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		ctrl->MouseUp(lParam);
		return 0;
	}
	case WM_KEYDOWN:
	{
		if (ctrl->KeyDown(wParam)) {
			return 0;
		}
		break;
	}
	case WM_KEYUP:
	{
		if (ctrl->KeyUp(wParam)) {
			return 0;
		}
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	}
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
		input.key_w = true;
		return true;
	case VK_S:
		input.key_s = true;
		return true;
	case VK_A:
		input.key_a = true;
		return true;
	case VK_D:
		input.key_d = true;
		return true;
	}

	return FALSE;
}

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

	UntransformedColouredVertex vertices[] = {
		{ 0.0f, 1.0f, 0.0f, 0xffff0000 },
		{ 1.0f, -1.0f, 0.0f, 0xff00ff00 },
		{ -1.0f, -1.0f, 0.0f, 0xffffff00 }
	};

	// Nothing changes at all in how we make the vertex buffer. You can thank all that
	// abstract code we set up for that.
	r = device->CreateVertexBuffer(sizeof(vertices), D3DUSAGE_WRITEONLY, vertices[0].FORMAT, D3DPOOL_DEFAULT, &vertexBuffer_, NULL);
	Errors::ErrorCheck(r, TEXT("Initialize resources failed to create vertex buffer"));

	// Move vertices into the buffer.
	void* bufferMemory;
	r = vertexBuffer_->Lock(0, sizeof(vertices), &bufferMemory, NULL);
	Errors::ErrorCheck(r, TEXT("Initialize resources failed to get a vertexBuffer lock"));

	memcpy(bufferMemory, vertices, sizeof(vertices));
	vertexBuffer_->Unlock();

	// Tell D3D what vertex format we're using, and to use our new buffer as the stream source.
	device->SetFVF(vertices[0].FORMAT);
	device->SetStreamSource(0, vertexBuffer_, 0, vertices[0].STRIDE_SIZE);
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

	float ms = model.getFrameTime();
	Camera& cam = model.getCamera();

	float factor = ms * SENSITIVITY * 0.0001f;

	if (input.key_w) {
		cam.setPos(cam.getPos() + cam.getDirection() * factor);
	}
	if (input.key_s) {
		cam.setPos(cam.getPos() - cam.getDirection() * factor);
	}
	if (input.key_a) {
		cam.setPos(cam.getPos() + cam.getRight() * factor);
	}
	if (input.key_d) {
		cam.setPos(cam.getPos() - cam.getRight() * factor);
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

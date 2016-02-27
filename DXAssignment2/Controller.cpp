#include "Controller.h"

//Static
long CALLBACK Controller::windowLoop(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	LPCREATESTRUCT cs;
	static Controller* ctrl;
	static int bMouseDown = 0;

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
		bMouseDown = 1;
		ctrl->MouseDown(lParam);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		if (bMouseDown) {
			ctrl->MouseMove(lParam);
		}
		return 0;
	}
	case WM_LBUTTONUP:
	{
		if (bMouseDown) {
			bMouseDown = 0;
			ctrl->MouseUp(lParam);
		}
		return 0;
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
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);

	//Errors::SetError(TEXT("Mouse down at (%d, %d)"), xPos, yPos);
}

void Controller::MouseMove(LPARAM lParam) {
	POINT p;
	p.x = GET_X_LPARAM(lParam);
	p.y = GET_Y_LPARAM(lParam);
}

void Controller::MouseUp(LPARAM lParam) {
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
}

int Controller::GameStartup() {
	if (FAILED(renderEngine.startEngine(hWnd, gameModel))) {
		Errors::SetError(TEXT("Initialization Failed"));
		GameShutdown();
		return E_FAIL;
	}

	//Initialize background image
	Background* bg = new Background(renderEngine.getDevice());
	bg->setImage(TEXT(DEFAULT_BITMAP));
	std::shared_ptr<Drawable2D> drawable(bg);
	gameModel.addBG(drawable);

	//Initialize frame counter
	gameModel.initFrameTimer();
	FrameRate* tw = new FrameRate(renderEngine.getDevice(), TEXT("font.bmp"), 10, 12, &gameModel);
	tw->setPosition(10, 10);
	tw->setTransparentColor(D3DCOLOR_ARGB(0, 255, 0, 255));
	std::shared_ptr<Drawable2D> drawableText(tw);
	gameModel.addFG(drawableText);

	return S_OK;
}

int Controller::GameLoop() {
	renderEngine.render(gameModel);
	gameModel.setFrameTick();

	if (GetAsyncKeyState(VK_ESCAPE))
		PostQuitMessage(0);

	return S_OK;
}

int Controller::GameShutdown() {
	renderEngine.stopEngine();
	return S_OK;
}

Controller::Controller(HINSTANCE hInstance)
	: hInstance(hInstance) {
}

Controller::~Controller() {
}

void Controller::setHWnd(const HWND newHWnd) {
	hWnd = newHWnd;
}
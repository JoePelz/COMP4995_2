#include "Model.h"

/*
Summary:
	Sets the display mode to a new resolution and whether it's windowed.
Params: 
	width: the new width in pixels. Must match a standard resolution in fullscreen mode.
	height: The new height in pixels. Must match a standard resolution in fullscreen mode.
	fullscreen: true if fullscreen mode. false if windowed.
Return: -
*/
void Model::setDisplayMode(int width, int height, bool fullscreen) {
	width_ = width;
	height_ = height;
	fullscreen_ = fullscreen;
}

/*
Summary:
	Get the current frame rate in frames per second.
Params: -
Return: The frame rate in frames per second.
*/
UINT Model::getFrameRate() const {
	return frameRate_;
}

/*
Summary:
	Get the time per frame in milliseconds
Params: -
Return: The time per frame in milliseconds
*/
float Model::getFrameTime() const {
	return (float)frameTime_ / ticksFrequency_ * 1000;
}


/*
Summary:
	Indicate that a frame has just happened, to update timing information.
Params: -
Return: -
*/
void Model::setFrameTick() {
	static INT64 lastTime = 0;
	INT64 newTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&newTime);

	frameTime_ = newTime - lastTime;
	frameRate_ = (UINT)((frameRate_ * (FRAME_RATE_SMOOTHING - 1) + ticksFrequency_ / (newTime-lastTime)) / FRAME_RATE_SMOOTHING); //running average of last n values.

	lastTime = newTime;
}

/*
Summary:
	Initialize timing information.
Params: -
Return: S_OK on success and E_FAIL if high resolution timing is not available.
*/
int Model::initFrameTimer() {
	QueryPerformanceFrequency((LARGE_INTEGER*)&ticksFrequency_);

	if (ticksFrequency_ == 0) {
		Errors::SetError(TEXT("The system does not support high resolution timing"));
		return E_FAIL;
	}

	return S_OK;
}
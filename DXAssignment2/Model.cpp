#include "Model.h"

Model::Model() : frameRate_{ 1000 }, width_{ DEFAULT_WIDTH }, height_{ DEFAULT_HEIGHT } { }

Model::~Model() {
}

UINT Model::getFrameRate() const {
	return frameRate_;
}

INT64 Model::getFrameTime() const {
	return frameTime_;
}
void Model::setFrameTick() {
	static INT64 lastTime = 0;
	INT64 newTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&newTime);

	frameTime_ = (frameTime_ * (FRAME_RATE_SMOOTHING - 1) + newTime - lastTime) / FRAME_RATE_SMOOTHING; //running average of last n values.
	frameRate_ = (UINT)((frameRate_ * (FRAME_RATE_SMOOTHING - 1) + ticksFrequency_ / (newTime-lastTime)) / FRAME_RATE_SMOOTHING); //running average of last n values.

	lastTime = newTime;
}

int Model::initFrameTimer() {
	QueryPerformanceFrequency((LARGE_INTEGER*)&ticksFrequency_);

	if (ticksFrequency_ == 0) {
		Errors::SetError(TEXT("The system does not support high resolution timing"));
		return E_FAIL;
	}

	return S_OK;
}

void Model::addBG(std::shared_ptr<Drawable2D> drawable) {
	bgLayers_.push_back(drawable);
}
const std::vector<std::shared_ptr<Drawable2D>>& Model::getBG() const {
	return bgLayers_;
}
void Model::clearBG() {
	bgLayers_.clear();
}
void Model::addFG(std::shared_ptr<Drawable2D> drawable) {
	fgLayers_.push_back(drawable);
}
const std::vector<std::shared_ptr<Drawable2D>>& Model::getFG() const {
	return fgLayers_;
}
void Model::clearFG() {
	bgLayers_.clear();
}
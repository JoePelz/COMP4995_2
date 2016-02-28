#include "Model.h"

Model::Model() : frameRate_{ 1000 }, width_{ DEFAULT_WIDTH }, height_{ DEFAULT_HEIGHT }, fullscreen_{ USE_FULLSCREEN }, selected_{&camera_} {
	camera_.setPosition({ 0.0f, 1.7f, -5.0f });
}

Model::~Model() {}

void Model::setDisplayMode(int width, int height, bool fullscreen) {
	width_ = width;
	height_ = height;
	fullscreen_ = fullscreen;
}

UINT Model::getFrameRate() const {
	return frameRate_;
}

float Model::getFrameTime() const {
	return (float)frameTime_ / ticksFrequency_ * 1000;
}
void Model::setFrameTick() {
	static INT64 lastTime = 0;
	INT64 newTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&newTime);

	frameTime_ = newTime - lastTime;
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

void Model::add3D(std::shared_ptr<Drawable3D> object) {
	polyLayers_.push_back(object);
}
const std::vector<std::shared_ptr<Drawable3D>>& Model::get3D() const {
	return polyLayers_;
}
void Model::clear3D() {
	polyLayers_.clear();
}
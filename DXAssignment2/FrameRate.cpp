#include "FrameRate.h"



FrameRate::FrameRate(LPDIRECT3DDEVICE9& device, const TCHAR* path, int fontWidth, int fontHeight, Model* model)
	: TextWriter(device, path, fontWidth, fontHeight), model_(model) 
{
}


FrameRate::~FrameRate() {
}

int FrameRate::draw(LPDIRECT3DSURFACE9 pBackSurf) {
	std::ostringstream os;
	os << "fps: " << model_->getFrameRate() << "\ntime: " << model_->getFrameTime() << "ms";
	setText(os.str());
	TextWriter::draw(pBackSurf);

	return S_OK;
}

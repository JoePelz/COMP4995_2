#include "FrameRate.h"

/*
Summary:
	Constructor. Creates a new empty FrameRate display. 
Params: 
	device: The device to draw on
	path: The path to the bitmap font
	fontWidth: the width of a character in the font used
	fontHeight: the height of a character in the font used
	model: the game model holding the frame rate information to display
Return: -
*/
FrameRate::FrameRate(const TCHAR* path, int fontWidth, int fontHeight, Model* model)
	: TextWriter(path, fontWidth, fontHeight), model_(model) { }

/*
Summary:
	Draws the frame rate to the screen.
Params: 
	pBackSurf: the buffer to draw onto
Return: -
*/
int FrameRate::draw(LPDIRECT3DDEVICE9& device, LPDIRECT3DSURFACE9 pBackSurf) {
	std::ostringstream os;
	os << "fps: " << model_->getFrameRate() << "\ntime: " << model_->getFrameTime() << "ms";
	setText(os.str());
	TextWriter::draw(device, pBackSurf);
	return S_OK;
}

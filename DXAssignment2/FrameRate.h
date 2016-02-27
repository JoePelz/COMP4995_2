#pragma once
#include "TextWriter.h"
#include "Model.h"
#include <sstream>

/* 2D element, writes the frame rate onto the screen. */
class FrameRate : public TextWriter {
	const Model* model_;
public:
	/* Creates a new frameRate display
	   args:
	     device: The device to draw on
		 path: The path to the bitmap font
		 fontWidth: the width of a character in the font used
		 fontHeight: the height of a character in the font used
		 model: the game model holding the frame rate information to display 
	*/
	FrameRate(LPDIRECT3DDEVICE9& device, const TCHAR* path, int fontWidth, int fontHeight, Model* model);
	~FrameRate();
	//Writes the frame rate, and time/frame onto the given back buffer.
	virtual int draw(LPDIRECT3DSURFACE9 pBackSurf) override;
};


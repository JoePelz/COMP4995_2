#pragma once

#include <Windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include "Errors.h"
#include "Drawable2D.h"

/*
The TextWriter class will write arbitrary ascii text on the screen 
using a bitmapped font supplied during construction.
*/

class TextWriter : public Drawable2D {
	LPDIRECT3DSURFACE9 alphabet_; //surface to hold the loaded font image
	int charWidth_;     //The width of 1 character in the font image
	int charHeight_;    //The height of 1 character in the font image
	int imgWidth_;      //The width of the font image
	int imgHeight_;     //The height of the font image
	int charsPerRow_;   //The number of chars per row of the font image
	BOOL bTransparent_; //True if font includes transparency
	D3DCOLOR colorKey_; //The font color to make transparent, if (bTransparent_)
	std::string text_;  //The message to write
	int posx_; //The X position to write at
	int posy_; //The Y position to write at

	//Load the font image at the given file path onto alphabet_.
	int LoadImageToSurface(LPDIRECT3DDEVICE9& device, const TCHAR* const pathname);
	//Print a single character of the font into the given pixel array.
	void PrintChar(int x, int y, const char Character, DWORD* pDestData, int DestPitch) const;
public:
	//Instantiate a TextWriter to write with the given font image(path) with characters of the given size (fontWidth x fontHeight)
	TextWriter(LPDIRECT3DDEVICE9& device, const TCHAR* path, int fontWidth, int fontHeight);
	~TextWriter();
	//Set the color to make transparent in the font
	void setTransparentColor(D3DCOLOR colorKey);
	//Set the string to write out
	void setText(const std::string& text);
	//Set the string to write out
	void setText(const std::string&& text);
	//Set the position on the screen to write at
	void setPosition(int x, int y);
	//Writes the current message (setText) onto the given back buffer.
	virtual int draw(LPDIRECT3DSURFACE9 pBackSurf) override;
};


#include "TextWriter.h"



/*
Summary:
	Constructor: Create a TextWriter based on the given ascii font image.
Params: 
	device: the device to draw in
	path: the path to the ascii image
	fontWidth: the width of a character in the font image
	fontHeight: the height of a character in the font image
Return: -
*/
TextWriter::TextWriter(LPDIRECT3DDEVICE9& device, const TCHAR* path, int fontWidth, int fontHeight)
	: alphabet_(NULL),
	charWidth_(fontWidth),
	charHeight_(fontHeight),
	imgWidth_(0),
	imgHeight_(0),
	charsPerRow_(0)
{
	LoadImageToSurface(device, path);
}

/*
Summary:
	Release resources for the font.
Params: -
Return: -
*/
TextWriter::~TextWriter() {
	if (alphabet_ != NULL)
		alphabet_->Release();
}

/*
Summary:
	Load an image from file, using the given device.
Params:
	device: the Direct3D device to create a surface with.
	pathname: The path to the image file (bmp only)
Return: -
*/
int TextWriter::LoadImageToSurface(LPDIRECT3DDEVICE9& device, const TCHAR* const pathname) {
	HRESULT r;
	HBITMAP hBitmap;
	BITMAP Bitmap;

	hBitmap = (HBITMAP)LoadImage(NULL, pathname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap == NULL) {
		Errors::SetError(TEXT("Unable to load bitmap"));
		return E_FAIL;
	}

	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	DeleteObject(hBitmap);//we only needed it for the header info to create a D3D surface

	//create surface for bitmap
	r = device->CreateOffscreenPlainSurface(Bitmap.bmWidth, Bitmap.bmHeight, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &alphabet_, NULL);
	imgWidth_ = Bitmap.bmWidth;
	imgHeight_ = Bitmap.bmHeight;
	charsPerRow_ = imgWidth_ / charWidth_;

	if (FAILED(r)) {
		Errors::SetError(TEXT("Unable to create surface for bitmap load"));
		return E_FAIL;
	}

	//load bitmap onto surface
	r = D3DXLoadSurfaceFromFile(alphabet_, NULL, NULL, pathname, NULL, D3DX_DEFAULT, 0, NULL);
	if (FAILED(r)) {
		Errors::SetError(TEXT("Unable to load file to surface"));
		return E_FAIL;
	}

	return S_OK;
}

/*
Summary:
	Set which color to treat as transparent in the font image. use NULL to disable transparency.
Params: 
	colorKey: the color to treat as transparent in the font image. Use D3DCOLOR_ARGB() or D3DCOLOR_XRGB() macros to generate
Return: -
*/
void TextWriter::setTransparentColor(D3DCOLOR colorKey) {
	bTransparent_ = (colorKey != NULL);
	colorKey_ = colorKey;
}

/*
Summary:
	Set what text to display on the screen.
Params:
	text: the text to write on the screen.
Return: -
*/
void TextWriter::setText(const std::string& text) {
	text_ = text;
}

/*
Summary:
	Set what text to display on the screen.
Params:
	text: the text to write on the screen.
Return: -
*/
void TextWriter::setText(const std::string&& text) {
	text_ = move(text);
}

/*
Summary:
	Set the position on the screen to draw the text at. Represents top left corner of text.
Params: 
	x: the x position to start at in pixels
	y: the y position to start at in pixels.
Return: -
*/
void TextWriter::setPosition(int x, int y) {
	posx_ = x;
	posy_ = y;
}

/*
Summary:
	Draw a single character onto a buffer.
Params: 
	x: the x coordinate in pixels to draw at
	y: the y coordinate in pixels to draw at
	Character: the char to draw
	pDestData: the buffer to draw on
	DestPitch: the number of elements wide pDestData. A.k.a. `stride`.
Return: -
*/
void TextWriter::PrintChar(int x, int y, const char Character, DWORD* pDestData, int DestPitch) const {
	HRESULT r = 0;

	div_t Result;	// Holds the result of divisions

	// The offset into the alphabet image
	int OffsetX = 0, OffsetY = 0;

	POINT LetterDestPoint = { 0, 0 };	// The destination point for the letter
	RECT LetterRect = { 0, 0, 0, 0 };	// The source rectangle for the letter

										
	if (!alphabet_) // If the alphabet has not been loaded yet then exit
		return;

	// The characters are specified in ASCII code, which begins at 32 so
	// we want to decrement this value by 32 to make it zero based
	//Character -= 32;

	// Avoid divide by 0 errors
	if (Character == 0)
		return;

	// Divide the character code by the number of letters per row.
	// The quotient will help get the vertical offset and the
	// remainder will help get the horizontal offset
	Result = div(Character, charsPerRow_);

	// Get the horizontal offset by multiplying the remainder
	// by the width of the Letter
	OffsetX = Result.rem * charWidth_;
	// Get the vertical offset by multiplying the quotient
	// by the height of the letter
	OffsetY = Result.quot * charHeight_;

	// Fill in the source rectangle with the computed offsets
	SetRect(&LetterRect, OffsetX, OffsetY,
		OffsetX + charWidth_, OffsetY + charHeight_);

	// Fill in the destination point
	LetterDestPoint.x = x;
	LetterDestPoint.y = y;

	D3DLOCKED_RECT LockedAlphabet;	// Holds info about the alphabet surface

									// Lock the source surface
	r = alphabet_->LockRect(&LockedAlphabet, 0, D3DLOCK_READONLY);
	if (FAILED(r)) {
		Errors::SetError(TEXT("TextWriter::PrintChar - Couldnt lock alphabet surface for PrintChar()"));
		return;
	}

	// Get a DWORD pointer to each surface
	DWORD* pAlphaData = (DWORD*)LockedAlphabet.pBits;

	// Convert the BYTE pitch pointer to a DWORD ptr
	LockedAlphabet.Pitch /= 4;
	DestPitch /= 4;

	// Compute the offset into the alphabet
	int AlphaOffset = OffsetY * LockedAlphabet.Pitch + OffsetX;
	// Compute the offset into the destination surface
	int DestOffset = y * DestPitch + x;

	// Loop for each row in the letter
	for (int cy = 0; cy < charHeight_; cy++) {
		// Loop for each column in the letter
		for (int cx = 0; cx < charWidth_; cx++) {
			if (bTransparent_) {
				// If this alphabet pixel is not transparent
				if (pAlphaData[AlphaOffset] != colorKey_) {
					// Then copy the pixel to the destination
					pDestData[DestOffset] = pAlphaData[AlphaOffset];
				}

				// Increment the offsets to the next pixel
				AlphaOffset++;
				DestOffset++;
			} else {
				pDestData[DestOffset] = pAlphaData[AlphaOffset];
				// Increment the offsets to the next pixel
				AlphaOffset++;
				DestOffset++;
			}
		}

		// Move the offsets to the start of the next row
		DestOffset += DestPitch - charWidth_;
		AlphaOffset += LockedAlphabet.Pitch - charWidth_;
	}

	// Unlock the surface
	alphabet_->UnlockRect();
}

/*
Summary:
	Draw the current text onto the given surface
Params: 
	pBackSurf: the surface to draw onto.
Return: -
*/
int TextWriter::draw(LPDIRECT3DSURFACE9 pBackSurf) {
	HRESULT hr;
	D3DLOCKED_RECT LockedRect;//locked area of display memory(buffer really) we are drawing to
	DWORD* pData;
	int y = posy_;
	int x = posx_;

	//lock the back buffer, so we can edit the pixels
	hr = pBackSurf->LockRect(&LockedRect, NULL, 0);
	if (FAILED(hr)) {
		Errors::SetError(TEXT("TextWriter::draw Could not lock the back buffer"));
	}
	pData = (DWORD*)(LockedRect.pBits);

	// Loop for each character in the string
	for (std::string::size_type i = 0; i < text_.length(); i++) {
		// Print the current character
		if (text_[i] == '\n') {
			y += charHeight_;
			x = posx_;
			continue;
		}
		PrintChar(x, y, text_[i], pData, LockedRect.Pitch);
		x += charWidth_;
	}

	pBackSurf->UnlockRect();
	pData = 0;

	return S_OK;
}
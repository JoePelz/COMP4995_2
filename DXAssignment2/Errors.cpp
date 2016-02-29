#include "Errors.h"

void Errors::SetError(TCHAR* szFormat, ...) {
	TCHAR szBuffer[1024];
	va_list pArgList;

	va_start(pArgList, szFormat);

	_vsntprintf_s(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), 1024, szFormat, pArgList);

	va_end(pArgList);

	OutputDebugString(szBuffer);
	OutputDebugString(TEXT("\n"));
}

void Errors::ErrorCheck(HRESULT error, TCHAR * szFormat, ...) {
	if (error == D3D_OK) return;

	TCHAR szBuffer[512];
	va_list pArgList;
	va_start(pArgList, szFormat);
	_vsntprintf_s(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), 1024, szFormat, pArgList);
	va_end(pArgList);
	OutputDebugString(szBuffer);
	OutputDebugString(TEXT("\n"));

	LPTSTR text;
	if (error == D3DERR_INVALIDCALL) text = TEXT("D3DERR_INVALIDCALL: Invalid Call or Parameter!");
	else if (error == D3DXERR_INVALIDDATA) text = TEXT("D3DXERR_INVALIDDATA: Some data is invalid. Maybe file not found?");
	else if (error == D3DERR_NOTAVAILABLE) text = TEXT("D3DERR_NOTAVAILABLE: Parameter not supported!");
	else if (error == D3DERR_DEVICELOST) text = TEXT("D3DERR_DEVICELOST: Direct3D device is in a lost (not operational) state!");
	else if (error == D3DERR_OUTOFVIDEOMEMORY) text = TEXT("D3DERR_OUTOFVIDEOMEMORY: Out of Video Memory!");
	else if (error == E_OUTOFMEMORY) text = TEXT("E_OUTOFMEMORY: Out of Memory!");
	else {
		text = new TCHAR[128];
		wsprintf(text, TEXT("Unknown Error: %X"), error);
	}
	OutputDebugString(text);
	OutputDebugString(TEXT("\n"));

	LPTSTR errorNote = new TCHAR[1024];
	wsprintf(errorNote, TEXT("%s\r\n%s"), text, szBuffer);
	throw(errorNote);
}

Errors::Errors() {
}


Errors::~Errors() {
}

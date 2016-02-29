#pragma once

#include <Windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>

class Errors {
public:
	//Static helper function to send error messages to the debugger with string formatting.
	static void SetError(TCHAR* szFormat, ...);
	static void ErrorCheck(HRESULT error, TCHAR* szFormat, ...);
	Errors();
	~Errors();
};


#pragma once

#include <Windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>

/*
Errors contains static methods to allow for the unification of error-handling.
*/

class Errors {
public:
	//Static helper function to send error messages to the debugger with string formatting.
	static void SetError(TCHAR* szFormat, ...);
	//Static helper function to check error codes, print messages, and throw errors when they are received.
	static void ErrorCheck(HRESULT error, TCHAR* szFormat, ...);
};


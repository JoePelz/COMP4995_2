#pragma once

#include <Windows.h>
#include <tchar.h>

class Errors {
public:
	//Static helper function to send error messages to the debugger with string formatting.
	static void SetError(TCHAR* szFormat, ...);
	Errors();
	~Errors();
};


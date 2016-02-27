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

Errors::Errors() {
}


Errors::~Errors() {
}

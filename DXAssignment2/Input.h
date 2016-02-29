#pragma once

#include <Windows.h>

class Input {
public:
	POINT mpos;
	POINTFLOAT mdelta;
	short scrollAmount;
	bool key_f;
	bool key_w;
	bool key_s;
	bool key_a;
	bool key_d;
	bool key_e;
	bool key_c;
	bool lbutton;
	bool rbutton;
	bool mbutton;
};

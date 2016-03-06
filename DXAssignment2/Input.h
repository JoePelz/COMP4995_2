#pragma once

#include <Windows.h>

/*
The input class records the state of user input buttons and keys
*/

class Input {
public:
	POINT mpos; //The last recorded position of the mouse
	POINTFLOAT mdelta; //the change in mouse position between recordings (on mouse dragging)
	short scrollAmount; //the number of clicks the mousewheen has scrolled.

	//The following are true if that given key or button is currently known to be in a pressed state.

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


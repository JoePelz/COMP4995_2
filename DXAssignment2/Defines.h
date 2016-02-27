#pragma once

#define DEFAULT_BITMAP "fields_small.bmp"   //The image to load for the background
#define DEFAULT_WIDTH 640                    //default width
#define DEFAULT_HEIGHT 480                   //default height
#define CLEAR_COLOR D3DCOLOR_XRGB(0, 0, 25)  //background color (if no image)
#define FRAME_RATE_SMOOTHING 50              //how many frames to average over during frame rate display

#define VC_EXTRALEAN          //win32 switch to load fewer libraries
#define WIN32_LEAN_AND_MEAN   //win32 switch to load fewer libraries

#define WINDOWED_MODE 1    // 1 for windowed, 0 for fullscreen

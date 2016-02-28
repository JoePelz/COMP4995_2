#pragma once

#define DEFAULT_BITMAP "fields_small.bmp"   //The image to load for the background
#define DEFAULT_WIDTH 640                    //default width
#define DEFAULT_HEIGHT 480                   //default height
#define CLEAR_COLOR D3DCOLOR_XRGB(0, 0, 100)  //background color (if no image)
#define FRAME_RATE_SMOOTHING 50              //how many frames to average over during frame rate display
#define SENSITIVITY 50                       //How responsive the program is to input (e.g. translating)


#define VC_EXTRALEAN          //win32 switch to load fewer libraries
#define WIN32_LEAN_AND_MEAN   //win32 switch to load fewer libraries

#define USE_FULLSCREEN 0    // 0 for windowed, 1 for fullscreen

#define VK_F 0x0046
#define VK_0 0x0030
#define VK_1 0x0031
#define VK_2 0x0032
#define VK_3 0x0033

#define VK_W 0x0057
#define VK_S 0x0053
#define VK_A 0x0041
#define VK_D 0x0044

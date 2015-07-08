// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include "GameException.h"
#include "DirectX.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "RTTI.h"

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define DeleteObject(object) if((object) != nullptr) { delete object; object = nullptr; }
#define DeleteObjects(objects) if((objects) != nullptr) { delete[] objects; objects = nullptr; }
#define ReleaseObject(object) if((object) != nullptr) { object->Release(); object = nullptr; }


using namespace DirectX;
using namespace DirectX::PackedVector;


// TODO: reference additional headers your program requires here

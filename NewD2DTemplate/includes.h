#pragma once

#include <wincodec.h>
//#pragma comment(lib, "windowscodec.lib")
#include <windows.h>

// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory>
#include <wchar.h>
#include <math.h>
#include <vector>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <time.h>
#include <sstream>
#include <iomanip>
#include <assert.h>
#include <algorithm>
#include "stdio.h"
#include <stdlib.h> 
#include <heapapi.h>

// user defined

#include "errors.h"
#ifndef class_static
#define class_static static
#endif
#ifndef local_persist
#define local_persist static
#endif
#ifndef global_variable
#define global_variable static
#endif

#include <unordered_map>


#define MAX_GROUPS 24

typedef std::vector<std::string> stringList;


template<class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
	)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

///////////////////////////////////////////////////////////
// Image Tages
#define LEVEL1_SPRITES 0


//////////////////////////////////////////////////////////

const static int TILE_WIDTH = 64;
const static int TILE_HEIGHT = 64;

#ifndef D2_MAT3x2_IDENTITY
#define D2_MATRIX_IDENTITY(m) { (m) = (D2D1::Matrix3x2F::Identity()); }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

template <typename T>
static void delete_ptr(T*  ptr)
{
	SAFE_DELETE(ptr);
}

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif



#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


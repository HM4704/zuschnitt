// Include this into your project stdafx.h file
#ifndef PRINT_EXTENSION_LIBRARY
#define PRINT_EXTENSION_LIBRARY

#if 0
#ifdef _PRINT_EXTENSION_LIBRARY_EXPORT
#define _PE_API __declspec(dllexport)
#else
#define _PE_API __declspec(dllimport)
#ifndef _PRINT_EXTENSION_NOAUTOLIB
#ifdef _DEBUG
#pragma comment(lib, "..\\dll\\Debug\\PrintExtensiond.lib")
#pragma message("Automatically linking to PrintExtensiond.dll")
#else
#pragma comment(lib, "..\\dll\\Release\\PrintExtension.lib")
#pragma message("Automatically linking to PrintExtension.dll")
#endif
#endif
#endif
#else
#define _PE_API
#endif // 0

// actual class declarations
#include "PrintPreviewDialog.h"

#endif // PRINT_EXTENSION_LIBRARY

#pragma once

#include <d3d9.h>
#include <d3dx9.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <vector>
#include<fstream>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include<stdlib.h>
#include<crtdbg.h>

#ifndef DBG_NEW

#define DBG_NEW new (_NORMAL_BLOCK , __FILE__, __LINE__)
#define new DBG_NEW

#endif

#endif //_DEBUG
// TODO: reference addit
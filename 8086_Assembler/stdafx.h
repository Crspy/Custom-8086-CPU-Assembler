// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <tchar.h>
#include <iostream>
#include <ios>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#define VERBOSE

#ifdef VERBOSE
#define logger(x) std::cout << (x) << "\n"
#else
#define logger(x) //
#endif



// reference additional headers your program requires here

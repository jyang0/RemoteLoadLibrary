#pragma once

#include <cstdlib>
#include <string>
#include <vector>

#include <windows.h>
#include <TlHelp32.h>

#ifndef _WIN64
#define PTR_TYPE int
#define PTR_DWORD_TYPE DWORD
#else
#define PTR_TYPE long long
#define PTR_DWORD_TYPE long long
#endif

using namespace std;
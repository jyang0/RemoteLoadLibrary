#pragma once

#include "stdafx.h"

extern "C" __declspec(dllexport) PTR_DWORD_TYPE WINAPI RemoteLoadLibrary(HANDLE hProcess, wstring dllname);
extern "C" __declspec(dllexport) BOOL WINAPI RemoteFreeLibrary(HANDLE hProcess, wstring dllname);
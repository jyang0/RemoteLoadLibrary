#include "stdafx.h"
#include "remote.h"

PTR_DWORD_TYPE WINAPI RemoteLoadLibrary(HANDLE hProcess, wstring dllname)
{
	void *pLibName = VirtualAllocEx(hProcess, 0, dllname.length() * sizeof(wchar_t) + sizeof(wchar_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!pLibName) {
		return 0; // error
	}

	BOOL hResult = WriteProcessMemory(hProcess, pLibName, dllname.c_str(), dllname.length() * sizeof(wchar_t) + sizeof(wchar_t), 0);
	if (!hResult) {
		return 0; // error
	}

	HANDLE hNewThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW"),
		pLibName, 0, 0);
	if (!hNewThread) {
		return 0; // error
	}

	PTR_DWORD_TYPE LibraryBaseAddress;

	WaitForSingleObject(hNewThread, INFINITE);
	GetExitCodeThread(hNewThread, (LPDWORD)&LibraryBaseAddress);

	VirtualFreeEx(hProcess, pLibName, dllname.length() * sizeof(wchar_t) + sizeof(wchar_t), MEM_RELEASE);
	CloseHandle(hNewThread);
	CloseHandle(hProcess);


	return LibraryBaseAddress;
}

BOOL WINAPI RemoteFreeLibrary(HANDLE hProcess, wstring dllname)
{
	void *pLibName = VirtualAllocEx(hProcess, 0, dllname.length() * sizeof(wchar_t) + sizeof(wchar_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!pLibName) {
		return 0; // error
	}

	BOOL hResult = WriteProcessMemory(hProcess, pLibName, dllname.c_str(), dllname.length() * sizeof(wchar_t) + sizeof(wchar_t), 0);
	if (!hResult) {
		return 0; // error
	}

	HANDLE hGetHandleThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "GetModuleHandleW"),
		pLibName, 0, 0);
	if (!hGetHandleThread) {
		return 0; // error
	}

	PTR_DWORD_TYPE LibraryBaseAddress;

	WaitForSingleObject(hGetHandleThread, INFINITE);
	GetExitCodeThread(hGetHandleThread, (LPDWORD)&LibraryBaseAddress);

	HANDLE hFreeLibThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "FreeLibrary"),
		(LPVOID)LibraryBaseAddress, 0, 0);
	if (!hFreeLibThread) {
		return 0; // error
	}

	BOOL result;

	WaitForSingleObject(hFreeLibThread, INFINITE);
	GetExitCodeThread(hFreeLibThread, (LPDWORD)&result);

	VirtualFreeEx(hProcess, pLibName, dllname.length() * sizeof(wchar_t) + sizeof(wchar_t), MEM_RELEASE);
	CloseHandle(hGetHandleThread);
	CloseHandle(hFreeLibThread);
	CloseHandle(hProcess);

	return result;
}
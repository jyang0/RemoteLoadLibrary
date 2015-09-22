#include "stdafx.h"
#include "remote.h"
#include "utils.h"

extern "C" __declspec(dllexport) void CALLBACK InjectByNameW(HWND hwnd, HINSTANCE hinst, LPWSTR lpszCmdLine, int nCmdShow);
extern "C" __declspec(dllexport) void CALLBACK UnloadByNameW(HWND hwnd, HINSTANCE hinst, LPWSTR lpszCmdLine, int nCmdShow);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBoxW(0, L"proc attach", L"proc attach", 0);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}



void CALLBACK UnloadByNameW(HWND hwnd, HINSTANCE hinst, LPWSTR lpszCmdLine, int nCmdShow)
{
	MessageBoxW(0, L"rundll32 unloadbyname entry point", L"test", 0);
	wstring param = lpszCmdLine;
	wstring procname = param.substr(0, param.find(L'/'));
	wstring dllname = param.substr(param.find(L'/') + 1);
	vector<int> pids;
	if (GetPIDsFromName(procname, pids)) {
		AcquireDebugPrivilege();
		for (int i = 0; i < pids.size(); i++) {
			HANDLE hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
				PROCESS_VM_READ | PROCESS_VM_WRITE, 0, pids[i]);
			RemoteFreeLibrary(hProcess, dllname);
		}
	}

}

void CALLBACK InjectByNameW(HWND hwnd, HINSTANCE hinst, LPWSTR lpszCmdLine, int nCmdShow)
{
	MessageBoxW(0, L"rundll32 injectbyname entry point", L"test", 0);
	wstring param = lpszCmdLine;
	wstring procname = param.substr(0, param.find(L'/'));
	wstring dllname = param.substr(param.find(L'/') + 1);
	vector<int> pids;
	if (GetPIDsFromName(procname, pids)) {
		AcquireDebugPrivilege();
		for (int i = 0; i < pids.size(); i++) {
			HANDLE hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
				PROCESS_VM_READ | PROCESS_VM_WRITE, 0, pids[i]);
			RemoteLoadLibrary(hProcess, dllname);
		}
	}
}


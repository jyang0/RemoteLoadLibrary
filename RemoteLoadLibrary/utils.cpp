#include "stdafx.h"
#include "utils.h"

int GetPIDsFromName(wstring procname, vector<int>& PIDs) // proc name, out output pids, size in number of pids, out npids number of pids there were
{
	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(PROCESSENTRY32W);

	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32FirstW(snap, &entry)) {
		do {
			if (!_wcsicmp(procname.c_str(), entry.szExeFile)) {
				PIDs.push_back(entry.th32ProcessID);
			}
		} while (Process32NextW(snap, &entry));
	}

	CloseHandle(snap);

	return TRUE; // for success
}

BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege) // returns true if successful
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable debug privilege

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

	{
		return FALSE;
	}

	return TRUE;
}

BOOL SetCurrentProcessPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege) // true if successful
{
	HANDLE hProcess = GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		if (SetPrivilege(hToken, lpszPrivilege, bEnablePrivilege)) {
			return TRUE;
		}
		else {
			return FALSE;
		}
		CloseHandle(hToken);
	}
	else {
		return FALSE;
	}
}

bool AcquireDebugPrivilege() // true if successful
{
	if (SetCurrentProcessPrivilege(SE_DEBUG_NAME, TRUE)) {
		return true;
	}
	else {
		return false;
	}
}
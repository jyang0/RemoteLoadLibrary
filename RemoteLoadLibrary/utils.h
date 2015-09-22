#pragma once

#include "stdafx.h"

bool AcquireDebugPrivilege();
BOOL SetCurrentProcessPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
int GetPIDsFromName(wstring procname, vector<int>& PIDs);

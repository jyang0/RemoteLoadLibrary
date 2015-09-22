# RemoteLoadLibrary
A library for loading and unloading DLLs in other processes

### Usage

#### Static linking
Add RemoteLoadLibrary.lib and remote.h to your project

#### Dynamic linking
Use LoadLibrary to import the following:
```
DWORD WINAPI RemoteLoadLibrary(HANDLE hProcess, wstring dllname);
BOOL WINAPI RemoteFreeLibrary(HANDLE hProcess, wstring dllname);
```

#### Using rundll32.exe
Use the following commands to directly load/unload a DLL in a process:
```
rundll32 RemoteLoadLibrary.dll,InjectByName (process name)/(dll name)
rundll32 RemoteLoadLibrary.dll,UnloadByName (process name)/(dll name)
```
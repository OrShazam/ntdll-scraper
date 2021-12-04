
#include "console.h"
#include "scrap.h"
#include "psapi.h"

void FillScraperData(PScraperData data, const char* dllName){
	static NTSTATUS(WINAPI *_GetModuleInformation)(
		IN  HANDLE hProcess,
		IN  HMODULE hModule,
		OUT LPMODULEINFO lpmodinfo,
		IN DWORD cb
	) = NULL;
	data->hModule = LoadLibraryA(dllName);
	if (data->hModule == NULL){
		PrintError("Can't find address of %s.\n", dllName);
		return;
	}
	if (!_GetModuleInformation){
		HMODULE kernel32 = LoadLibraryA("kernel32.dll");
		if (!kernel32){
			// shouldn't happen
			return;
		}
		FARPROC procPtr = GetProcAddress(kernel32,"GetModuleInformation");
		if (!procPtr){
			return;
		}
		_GetModuleInformation = (NTSTATUS(WINAPI*)(
			HANDLE,
			HMODULE,
			LPMODULEINFO,
			DWORD)		
		)procPtr;
	}
	MODULEINFO info;
	DWORD cb = sizeof(MODULEINFO);
	if (!_GetModuleInformation(GetCurrentProcess(),
		data->hModule,
		&info, cb)){
		PrintError("Can't retrieve module information.\n");
		return;
	}
	data->Base = info.lpBaseOfDll;
	data->ImageSize = info.SizeOfImage;
}

BOOL FindBytes(PScraperData data, BYTE* bytes, size_t sizeOfBytes, LPVOID* address){
	BOOL success = FALSE;
	LPVOID start = data->Base;
	for (;start < data->Base + data->ImageSize; (BYTE*)start++){
		if (success) break;
		for (size_t i = 0; i < sizeOfBytes; i++){
			if (bytes[i] != ((BYTE *)start)[i])
				break;
			if (i == sizeOfBytes-1)
				success = TRUE;
		}
	}
	if (success)
		*address = start;
	else 
		*address = NULL;
	return success;
}

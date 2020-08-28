#include <windows.h>

#define IMAGE_BASE 0x00400000
#define SUN_INCREMENT_ADDRESS IMAGE_BASE + 0X000309F0
#define SUN_AMOUNT_ADDRESS readDword(hPVZProcess,readDword(hPVZProcess, 0x6a9ec0) + 0x768) + 0x5560

DWORD readDword(HANDLE hPVZProcess, DWORD address) {
	DWORD result, dwret;
	ReadProcessMemory(hPVZProcess, (LPVOID)address, &result, sizeof(DWORD), &dwret);
	return result;
}

void writeDword(HANDLE hPVZProcess, DWORD address, DWORD value) {
	DWORD dwret;
	WriteProcessMemory(hPVZProcess, (LPVOID)address, &value, sizeof(DWORD), &dwret);
}
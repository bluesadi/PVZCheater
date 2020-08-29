#include <windows.h>

#define IMAGE_BASE 0x00400000
#define SUN_PICKED_UP_ONCE_ADDRESS 0x004309F0
#define SUN_ADDRESS readDword(hPVZProcess,readDword(hPVZProcess, 0x6a9ec0) + 0x768) + 0x5560
#define SUB_SUN_ADDRESS 0x0041BA74
#define MAX_SUN 9990
#define JUDGE_PLANT_PLACED_ADDRESS 0x0040FE2D //85 C0 test eax,eax

DWORD readDword(HANDLE hPVZProcess, DWORD address) {
	DWORD result, dwret;
	ReadProcessMemory(hPVZProcess, (LPVOID)address, &result, sizeof(DWORD), &dwret);
	return result;
}

void writeDword(HANDLE hPVZProcess, DWORD address, DWORD value) {
	DWORD dwret;
	WriteProcessMemory(hPVZProcess, (LPVOID)address, &value, sizeof(DWORD), &dwret);
}

BYTE readByte(HANDLE hPVZProcess, DWORD address) {
	BYTE result;
	DWORD dwret;
	ReadProcessMemory(hPVZProcess, (LPVOID)address, &result, sizeof(BYTE), &dwret);
	return result;
}

void writeByte(HANDLE hPVZProcess, DWORD address, BYTE value) {
	DWORD dwret;
	WriteProcessMemory(hPVZProcess, (LPVOID)address, &value, sizeof(BYTE), &dwret);
}
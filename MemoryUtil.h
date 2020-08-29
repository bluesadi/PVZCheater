#include <windows.h> //WIN32 API

#define IMAGE_BASE 0x00400000 //No ASLR
#define SUN_PICKED_UP_ONCE_ADDRESS 0x004309F0 //B9 19 00 00 00  mov ecx, 19h
#define SUN_ADDRESS readDword(hPVZProcess,readDword(hPVZProcess, 0x6a9ec0) + 0x768) + 0x5560 //dynamic address
#define SUB_SUN_ADDRESS 0x0041BA74 //2B F3  sub esi, ebx
#define MAX_SUN 9990 //00430A1D 81 F9 06 27 00 00  cmp ecx, 9990
#define JUDGE_PLANT_PLACED_ADDRESS 0x0040FE2D //85 C0  test eax, eax
#define SET_COOLDOWN_ADDRESS 0x00488E76 //C6 45 48 00  mov byte ptr [ebp+48h], 0

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
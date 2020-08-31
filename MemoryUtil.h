#include <windows.h> //WIN32 API

#define IMAGE_BASE 0x00400000 //No ASLR
#define SUN_PICKED_UP_ONCE_ADDRESS 0x004309F0 //B9 19 00 00 00  mov ecx, 19h
#define SUN_ADDRESS readDword(hPVZProcess,readDword(hPVZProcess, 0x6a9ec0) + 0x768) + 0x5560 //dynamic address
#define SUB_SUN_ADDRESS 0x0041BA74 //2B F3  sub esi, ebx
#define MAX_SUN 9990 //00430A1D 81 F9 06 27 00 00  cmp ecx, 9990
#define JUDGE_PLANT_PLACED_ADDRESS 0x0040FE2D //85 C0  test eax, eax
#define SET_COOLDOWN_ADDRESS 0x00488E76 //C6 45 48 00  mov byte ptr [ebp+48h], 0
#define JUDGE_PICK_UP_SUN_ADDRESS 0x0043158F //75 08  jnz short loc_431599
#define REDUCE_PLANT_HEALTH_ADDRESS 0x0052FCF0 + 3 //83 46 40 FC  add dword ptr [esi+40h], 0FFFFFFFCh
#define REDUCE_ZOMBIE_ARMOR_ADDRESS 0x00531046 //F6 C3 04  test bl, 4
#define REDUCE_ZOMBIE_HEALTH_ADDRESS 0x0053130F //2B 7C 24 20  sub edi, [esp+18h+arg_4]

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

void writeBytes(HANDLE hPVZProcess, DWORD address, INT n, DWORD value, ...) {
	DWORD dwret;
	for (int i = 0; i < n; i++) {
		writeByte(hPVZProcess, address + i, (&value)[i]);
	}
}
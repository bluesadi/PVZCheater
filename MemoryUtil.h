#include <windows.h> //WIN32 API
#include <Tlhelp32.h>
#include <Psapi.h>

#define IMAGE_BASE 0x00400000 //No ASLR
#define SUN_PICKED_UP_ONCE_ADDRESS 0x004309F0 //B9 19 00 00 00  mov ecx, 19h
#define SUN_ADDRESS readDword(readDword(0x6a9ec0) + 0x768) + 0x5560 //dynamic address
#define SUB_SUN_ADDRESS 0x0041BA74 //2B F3  sub esi, ebx
#define MAX_SUN 9990 //00430A1D 81 F9 06 27 00 00  cmp ecx, 9990
#define JUDGE_PLANT_PLACED_ADDRESS 0x0040FE2D //85 C0  test eax, eax
#define SET_COOLDOWN_ADDRESS 0x00488E76 //C6 45 48 00  mov byte ptr [ebp+48h], 0
#define JUDGE_PICK_UP_SUN_ADDRESS 0x0043158F //75 08  jnz short loc_431599
#define REDUCE_PLANT_HEALTH_ADDRESS 0x0052FCF0 + 3 //83 46 40 FC  add dword ptr [esi+40h], 0FFFFFFFCh
#define REDUCE_ZOMBIE_ARMOR_ADDRESS 0x00531046 //F6 C3 04  test bl, 4
#define REDUCE_ZOMBIE_HEALTH_ADDRESS 0x0053130F //2B 7C 24 20  sub edi, [esp+18h+arg_4]
#define SET_PROJECTILE_TYPE_ADDRESS 0x0046C765
#define FUNCTION_RAND_ADDRESS 0x0061E087

HANDLE getPVZProcess() {
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	while (Process32Next(hSnapshot, &processInfo)) {
		if (!lstrcmp(processInfo.szExeFile, L"PlantsVsZombies.exe")) {
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, processInfo.th32ProcessID);
			return hProcess;
		}
	}
	return NULL;
}

HANDLE hPVZProcess = getPVZProcess();

DWORD readDword(DWORD address) {
	DWORD result, dwret;
	ReadProcessMemory(hPVZProcess, (LPVOID)address, &result, sizeof(DWORD), &dwret);
	return result;
}

void writeDword(DWORD address, DWORD value) {
	DWORD dwret;
	WriteProcessMemory(hPVZProcess, (LPVOID)address, &value, sizeof(DWORD), &dwret);
}

BYTE readByte(DWORD address) {
	BYTE result;
	DWORD dwret;
	ReadProcessMemory(hPVZProcess, (LPVOID)address, &result, sizeof(BYTE), &dwret);
	return result;
}

void writeByte(DWORD address, BYTE value) {
	DWORD dwret;
	WriteProcessMemory(hPVZProcess, (LPVOID)address, &value, sizeof(BYTE), &dwret);
}

void writeBytes(DWORD address, INT n, DWORD value, ...) {
	DWORD dwret;
	for (int i = 0; i < n; i++) {
		writeByte(address + i, (&value)[i]);
	}
}

void writeCall(DWORD from, DWORD to) {
	DWORD rel32 = to - (from + 5);
	BYTE* lprel32 = (BYTE*)&rel32;
	writeBytes(from, 5, 0xE8, lprel32[0], lprel32[1], lprel32[2], lprel32[3]);
}

void writeJmpRel32(DWORD from,DWORD to) {
	DWORD rel32 = to - (from + 5);
	BYTE* lprel32 = (BYTE*)&rel32;
	writeBytes(from, 5,0xE9, lprel32[0], lprel32[1], lprel32[2], lprel32[3]);
}
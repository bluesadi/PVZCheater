#include "MemoryUtil.h"
#include <stdio.h>
#include <Tlhelp32.h>
#include <Psapi.h>
#define print(x) printf("%s\n",x)
#define getInt(x) scanf("%d",&x)

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

LPCSTR COMMANDS[100] = {
	"0. Exit"
	"1. Modify sun",
	"2. Unlimited sun",
	"3. Multiple plants on one grid",
	"4. No cooldown",
	"5. Auto-pickup sun",
	"6. Unlimited health",
	"7. Highdamage"
};

int main() {
	HANDLE hPVZProcess = getPVZProcess();
	if (!hPVZProcess) {
		print("Can't find PVZ process.");
		exit(0);
	}
	print("What do you wanna do?");
	for (int i = 0; COMMANDS[i]; i++) {
		print(COMMANDS[i]);
	}
	while (true) {
		int command;
		printf("Please type the number: ");
		getInt(command);
		switch (command) {
			case -1:
				writeBytes(hPVZProcess, 0x00531046, 3, 0x31, 0xC9,0x90);
				writeBytes(hPVZProcess, 0x0053130F, 4, 0x31, 0xFF,0x90,0x90);
				break;
			case 0:
				print("Good bye~");
				exit(0);
			case 1:
				int amount;
				print("How many?");
				getInt(amount);
				if (amount >= 0) {
					writeDword(hPVZProcess, SUN_ADDRESS, amount);
					break;
				}
				print("Bad argument. Sun should be positive or zero.");
				break;
			case 2:
				writeDword(hPVZProcess, SUN_ADDRESS, MAX_SUN);
				writeBytes(hPVZProcess, SUB_SUN_ADDRESS,2, 0x90,0x90);
				print("Unlimited sun on.");
				break;
			case 3:
				writeByte(hPVZProcess, JUDGE_PLANT_PLACED_ADDRESS, 0x33); //85 C0  test eax, eax -> 33 C0  xor eax, eax
				break;
			case 4:
				writeByte(hPVZProcess, SET_COOLDOWN_ADDRESS, 0x01); //C6 45 48 00  mov byte ptr [ebp+48h], 0 -> C6 45 48 01  mov byte ptr [ebp+48h], 1
				break;
			case 5:
				writeByte(hPVZProcess, JUDGE_PICK_UP_SUN_ADDRESS, 0xEB); //75 08  jnz short loc_431599 -> EB 08  jmp short loc_431599
				break;
			case 6:
				writeByte(hPVZProcess, REDUCE_PLANT_HEALTH_ADDRESS, 0x00); //83 46 40 FC  add dword ptr [esi+40h], 0FFFFFFFCh -> 83 46 40 00  add dword ptr [esi+40h], 0
				break;
			case 7:
				writeBytes(hPVZProcess, REDUCE_ZOMBIE_ARMOR_ADDRESS, 3, 0x31, 0xC9, 0x90); //F6 C3 04  test bl, 4 -> 31 C9 90 xor ecx, ecx nop
				writeBytes(hPVZProcess, REDUCE_ZOMBIE_HEALTH_ADDRESS, 4, 0x31, 0xFF, 0x90, 0x90); //2B 7C 24 20  sub edi, [esp+18h+arg_4] -> xor edi,edi nop nop
			default:
				print("Wrong command.");
				break;
		}
	}
}
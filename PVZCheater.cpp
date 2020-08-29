#include "MemoryUtil.h"
#include "MessageUtil.h"
#include <Tlhelp32.h>
#include <Psapi.h>

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

const char* COMMANDS[100] = {
	"1. Modify sun",
	"2. Unlimited sun",
	"3. Multiple plants on one grid",
	"4. Exit"
};

int main() {
	HANDLE hPVZProcess = getPVZProcess();
	if (!hPVZProcess) {
		print("Can't find PVZ process.");
		exit(0);
	}
	print("What do you wanna do?");
	for (int i = 0; COMMANDS[i]; i++) {
		printf("%s\n",COMMANDS[i]);
	}
	while (true) {
		int command;
		printf("Please type the number: ");
		scanf("%d", &command);
		switch (command) {
			case 1:
				int amount;
				print("How many?");
				scanf("%d",&amount);
				if (amount >= 0) {
					writeDword(hPVZProcess, SUN_ADDRESS, amount);
					break;
				}
				wrongArg("Sun should be positive or zero.");
				break;
			case 2:
				if (readByte(hPVZProcess, SUB_SUN_ADDRESS) == 0x90) {
					writeByte(hPVZProcess, SUB_SUN_ADDRESS, 0x2B);
					writeByte(hPVZProcess, SUB_SUN_ADDRESS + 1, 0xF3);
					print("Unlimited sun off.");
					break;
				}
				writeDword(hPVZProcess, SUN_ADDRESS, MAX_SUN);
				writeByte(hPVZProcess, SUB_SUN_ADDRESS, 0x90);
				writeByte(hPVZProcess, SUB_SUN_ADDRESS + 1, 0x90);
				print("Unlimited sun on.");
				break;
			case 3:
				writeByte(hPVZProcess, JUDGE_PLANT_PLACED_ADDRESS, 0x33); //85 C0 test eax, eax -> 33 C0 xor eax, eax
				break;
			case 4:
				print("Good bye~");
				exit(0);
			default:
				print("Wrong command.");
				break;
		}
	}
}
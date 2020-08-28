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
	"2. Modify sun picked up once",
	"3. Exit"
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
				print("How many?");
				int amount;
				scanf("%d",&amount);
				if (amount >= 0) {
					writeDword(hPVZProcess, SUN_ADDRESS, amount);
					print("Modification success.");
				} else {
					wrongArg("Sun should be positive or zero.");
				}
				break;
			case 3:
				print("Good bye~");
				exit(0);
			default:
				print("Wrong command.");
				break;
		}
	}
}
#include <stdio.h>
#include "MemoryUtil.h"
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

int main() {
	HANDLE hPVZProcess = getPVZProcess();
	if (!hPVZProcess) {
		puts("Can't find PVZ process.\n");
		exit(0);
	}
	writeDword(hPVZProcess,SUN_INCREMENT_ADDRESS, 50);
	DWORD result = readDword(hPVZProcess, SUN_INCREMENT_ADDRESS);
	writeDword(hPVZProcess, SUN_AMOUNT_ADDRESS, 10000);
	DWORD sun = readDword(hPVZProcess, SUN_AMOUNT_ADDRESS);
	printf("%x", sun);
}
﻿#include "MemoryUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#define print(x) printf("%s\n",x)
#define getInt(x) scanf("%d",&x)

LPCSTR COMMANDS[100] = {
	"0. Exit",
	"1. Modify sun",
	"2. Unlimited sun",
	"3. Multiple plants on one grid",
	"4. No cooldown",
	"5. Auto-pickup sun",
	"6. Unlimited health",
	"7. Highdamage",
	"8. Colorful bullet"
};

void demo() {
	LPVOID addr = VirtualAllocEx(hPVZProcess, NULL, 100, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	writeByte(0x0046C765, 0xE9);
	writeJmpRel32(0x0046C765, (DWORD)addr);
	writeBytes(0x0046C76A, 2, 0x90, 0x90);
	/*
		call _rand
		push ebx
		push edx
		mov ebx, 0Dh
		div ebx
		mov eax, edx
		mov [ebp+5Ch], eax
		pop edx
		pop ebx
		jmp loc_46C76C
	*/
	writeCall((DWORD)addr, 0x0061E087);
	writeBytes((DWORD)addr + 5, 16, 0x53,0x52,0xBB,0x0D,0x00,0x00,0x00,0xF7,0xF3,0x89,0xD0,0x89,0x45,0x5C,0x5A,0x5B);
	writeJmpRel32((DWORD)addr + 21, 0x0046C76C);
	printf("%x\n", addr);
}

int main() {
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
				demo();
				break;
			case 0:
				print("Good bye~");
				exit(0);
			case 1:
				int amount;
				print("How many?");
				getInt(amount);
				if (amount >= 0) {
					writeDword(SUN_ADDRESS, amount);
					break;
				}
				print("Bad argument. Sun should be positive or zero.");
				break;
			case 2:
				writeDword(SUN_ADDRESS, MAX_SUN);
				writeBytes(SUB_SUN_ADDRESS,2, 0x90,0x90); //2B F3  sub esi, ebx -> 90 90  nop nop
				break;
			case 3:
				writeByte(JUDGE_PLANT_PLACED_ADDRESS, 0x33); //85 C0  test eax, eax -> 33 C0  xor eax, eax
				break;
			case 4:
				writeByte(SET_COOLDOWN_ADDRESS, 0x01); //C6 45 48 00  mov byte ptr [ebp+48h], 0 -> C6 45 48 01  mov byte ptr [ebp+48h], 1
				break;
			case 5:
				writeByte(JUDGE_PICK_UP_SUN_ADDRESS, 0xEB); //75 08  jnz short loc_431599 -> EB 08  jmp short loc_431599
				break;
			case 6:
				writeByte(REDUCE_PLANT_HEALTH_ADDRESS, 0x00); //83 46 40 FC  add dword ptr [esi+40h], 0FFFFFFFCh -> 83 46 40 00  add dword ptr [esi+40h], 0
				break;
			case 7:
				writeBytes(REDUCE_ZOMBIE_ARMOR_ADDRESS, 3, 0x31, 0xC9, 0x90); //F6 C3 04  test bl, 4 -> 31 C9 90 xor ecx, ecx nop
				writeBytes(REDUCE_ZOMBIE_HEALTH_ADDRESS, 4, 0x31, 0xFF, 0x90, 0x90); //2B 7C 24 20  sub edi, [esp+18h+arg_4] -> xor edi,edi nop nop
			case 8:
				LPVOID addr = VirtualAllocEx(hPVZProcess, NULL, 100, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
				writeByte(0x0046C765, 0xE9);
				writeJmpRel32(0x0046C765, (DWORD)addr);
				writeBytes(0x0046C76A, 2, 0x90, 0x90);
				/*
					call _rand
					push ebx
					push edx
					mov ebx, 0Dh
					div ebx
					mov eax, edx
					mov [ebp+5Ch], eax
					pop edx
					pop ebx
					jmp loc_46C76C
				*/
				writeCall((DWORD)addr, 0x0061E087);
				writeBytes((DWORD)addr + 5, 16, 0x53, 0x52, 0xBB, 0x0D, 0x00, 0x00, 0x00, 0xF7, 0xF3, 0x89, 0xD0, 0x89, 0x45, 0x5C, 0x5A, 0x5B);
				writeJmpRel32((DWORD)addr + 21, 0x0046C76C);
				break;
			default:
				print("Wrong command.");
				break;
		}
	}
}
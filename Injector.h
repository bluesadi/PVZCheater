#include "MemoryUtil.h"

void colorBullets() {
	LPVOID addr = VirtualAllocEx(hPVZProcess, NULL, 100, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE); //shellcode address
	writeJmpRel32(SET_PROJECTILE_TYPE_ADDRESS, (DWORD)addr); //jmp to shellcode
	writeBytes(SET_PROJECTILE_TYPE_ADDRESS + 5, 2, 0x90, 0x90);
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
	writeCall((DWORD)addr, FUNCTION_RAND_ADDRESS);
	writeBytes((DWORD)addr + 5, 16, 0x53, 0x52, 0xBB, 0x0D, 0x00, 0x00, 0x00, 0xF7, 0xF3, 0x89, 0xD0, 0x89, 0x45, 0x5C, 0x5A, 0x5B);
	writeJmpRel32((DWORD)addr + 21, SET_PROJECTILE_TYPE_ADDRESS + 7);
}
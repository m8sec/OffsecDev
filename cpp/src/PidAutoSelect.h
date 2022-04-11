#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <iostream>
#include <string>
#define MAX_NAME

/*
	Author: @m8r0wn
*/

class PidAutoSelect {
    public:
        int InjPid[MAX_NAME];   // Array of (potentialy )injectable PIDs
        int InjSize = 0;        // InjPid counter

    // Enumerate all PIDs, process, and return signle PID for inj
    int GetPid() 
    {
        DWORD aProcesses[1024], cbNeeded, cProcesses;
        unsigned int i;

        if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        {
            return 1;
        }

        // Calculate how many process identifiers were returned.
        cProcesses = cbNeeded / sizeof(DWORD);

        // Print the name and process identifier for each process.
        for ( i = 0; i < cProcesses; i++ )
        {
            if( aProcesses[i] != 0 )
            {
                // verify each process meets inj criteria
                ProcessPid( aProcesses[i] );
            }
        }

        // return random PID from array
        return InjPid[rand() % InjSize];
    }


    void ProcessPid( DWORD processID) {
        TCHAR szProcessName[MAX_PATH]   = TEXT("<unknown>");
        char lpDomain[MAX_NAME]         = "";
        char lpName[MAX_NAME]           = "<null>";

        // Get a handle to the process.
        HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                    PROCESS_VM_READ,
                                    FALSE, processID );

        // Get the process name & token information
        if (NULL != hProcess )
        {
            HMODULE hMod;
            DWORD cbNeeded;
            if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
            {
                // Get Process Name & SID
                GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR) );
                
                // Open Process Token
                HANDLE hToken = NULL;
                if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
                    
                    DWORD dwLength = MAX_NAME;
                    PTOKEN_USER ptu = (PTOKEN_USER)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY, dwLength);

                    // Get Token Information
                    DWORD dwSize = MAX_NAME;
                    if (GetTokenInformation(hToken, TokenUser, (LPVOID) ptu, dwSize, &dwLength)) {
                        
                        SID_NAME_USE SidType;
                        LookupAccountSid(NULL, ptu->User.Sid, lpName, (LPDWORD)&dwSize, lpDomain, &dwSize, &SidType);

                        if (EvalProc(lpName, szProcessName)) {
                            InjPid[InjSize] = processID;
                            InjSize = InjSize+1;
                        }
                    } 
                }
            }
        }
        CloseHandle( hProcess );
        return;
    }

    bool EvalProc(char Name[MAX_NAME], TCHAR ProcessName[MAX_NAME]){
        // Injection criteria
        if (Name != "<null>") 
        {
            if (!_tcscmp(ProcessName, _T("svchost.exe"))){
                return TRUE;
            }
        } 
        return FALSE;
    }
};
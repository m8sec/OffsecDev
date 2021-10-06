/*
    Author: @m8r0wn
	
    List all processes, PID, & owner information

    References:
        https://docs.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes
*/

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <iostream>

#define MAX_NAME 256

void PrintProcessDetails( DWORD processID )
{
    TCHAR szProcessName[MAX_PATH]   = TEXT("<unknown>");
    char lpDomain[MAX_NAME]         = "";
    char lpName[MAX_NAME]           = "<null>";

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

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
                } 
            } 
        }
    }

    _tprintf( TEXT("%-30s PID: %-10u User: %s\\%s\n"), szProcessName, processID, lpDomain, lpName);
    CloseHandle( hProcess );
}

int main() 
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
            PrintProcessDetails( aProcesses[i] );
        }
    }
    return 0;
}
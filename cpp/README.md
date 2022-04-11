<p align="center">
  <img src="../_resources/img/0001.png">
</p>

# OffsecDev: C++

### Table of Contents
  * [Examples](#examples)
  * [Development Environment](#development-environment)

## Examples
| File                                                   | Description                                                                                                                                                                                                                                                                                                               |
|--------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| [proclist.cpp](src/proclist.cpp)                       | Extends Microsoft's [EnumProcesses](https://docs.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes) example to list Name, PID, & owner information for each process.                                                                                                                                      |
| [virtualalloc_loader.cpp](src/virtualalloc_loader.cpp) | Injects shellcode in local process with ```VirtualAlloc```/```CreateThread```.                                                                                                                                                                                                                                            |
| [heapalloc_loader.cpp](src/heapalloc_loader.cpp)       | Injects shellcode in local process with ```HeapAlloc```/```CreateThread```.                                                                                                                                                                                                                                               |
| [classic_injection.cpp](src/classic_injection.cpp)     | Inject shellcode in remote process using ```VirtualAllocEx```/```CreateRemoteThread```. PID can be defined in cmd args or chosen automatically using ```PidAutoSelect```.                                                                                                                                                 |
| [suspended_injection.cpp](src/suspended_injection.cpp)          | Inject shellcode in remote process in a suspended state with protection ```PAGE_NOACCESS```. Sleeps to avoid Defender Runtime Detection before modifying protections and calling ```ResumeThread``` - *Based on [plackyhacker's Suspended-Thread-Injection](https://github.com/plackyhacker/Suspended-Thread-Injection)*. |


## Development Environment
The programs listed were created on Windows 10 using VS Code and the C/C++ extension. See the post below for setup & compiling instructions:
* [https://code.visualstudio.com/docs/languages/cpp](https://code.visualstudio.com/docs/languages/cpp)


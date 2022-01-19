<p align="center">
  <img src="https://user-images.githubusercontent.com/13889819/136234601-95d9e6e5-0353-49fb-9e0b-205359635700.png">
</p>
  
# OffsecDev
Working repo used to experiment with C++ as it relates to offensive security.

## Table of Contents
  * [Examples](#examples)
  * [Development Environment](#development-environment)

## Examples
| File | Description |
| ---- | ----------- |
| [proclist.cpp](process_explorer/proclist.cpp) | Extends Microsoft's [EnumProcesses](https://docs.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes) example to list Name, PID, & owner information for each process. |
| [VirtualAlloc_loader.cpp](shellcode_injection/VirtualAlloc_loader.cpp) | Load shellcode into the current process using ```VirtualAlloc```/```CreateThread```.|
| [HeapAlloc_loader.cpp](shellcode_injection/HeapAlloc_loader.cpp) | Load shellcode into the current process using ```HeapAlloc```/```CreateThread```. |
| [VirtualAllocEx_inj.cpp](shellcode_injection/VirtualAllocEx_inj.cpp) | Inject shellcode into a remote process using ```VirtualAllocEx```/```CreateRemoteThread```. PID can be defined in cmd args or chosen automatically using ```PidAutoSelect```. |
| [Suspended_inj.cpp](shellcode_injection/Suspended_inj.cpp) | Inject shellcode in a remote process using ```VirtualAllocEx```/```CreateRemoteThread``` in a suspended state with protection ```PAGE_NOACCESS```. Sleeps to avoid AV before modifying protections and calling ```ResumeThread``` - *Based on [plackyhacker's Suspended-Thread-Injection](https://github.com/plackyhacker/Suspended-Thread-Injection)*.|




## Development Environment
To this point, the programs listed were created on Windows 10 using VS Code and the C/C++ extension. See the post below for setup & compiling instructions:
* [https://code.visualstudio.com/docs/languages/cpp](https://code.visualstudio.com/docs/languages/cpp)


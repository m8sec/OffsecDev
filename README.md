<p align="center">
  <img src="https://user-images.githubusercontent.com/13889819/136234601-95d9e6e5-0353-49fb-9e0b-205359635700.png">
</p>
  
# LifeByTheCpp
Working repo used to experiment with C++ as it relates to offensive security.

## Table of Contents
  * [Examples](#examples)
  * [Development Environment](#development-environment)

## Examples
| File | Description |
| ---- | ----------- |
| [proclist.cpp](process_explorer/proclist.cpp) | Extends Microsoft's [EnumProcesses](https://docs.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes) example to list Name, PID, & owner information for each process. |
| [local_VirtualAlloc.cpp](shellcode_injection/local_VirtualAlloc.cpp) | Load shellcode into the running process using ```VirtualAlloc()```/```CreateThread()```. |
| [local_HeapAlloc.cpp](shellcode_injection/local_HeapAlloc.cpp) | Load shellcode into the running process using ```HeapAlloc()```/```CreateThread()```. |
| [remote_VirtualAllocEx.cpp](shellcode_injection/remote_VirtualAllocEx.cpp) | Inject shellcode into a remote process using ```VirtualAllocEx()```/```CreateRemoteThread()```. PID can be defined in cmd args or chosen automatically using ```PidAutoSelect()```. |




## Development Environment
To this point, the programs listed were created using VS Code and the C/C++ extension. See the post below for setup & compiling instructions:
* [https://code.visualstudio.com/docs/languages/cpp](https://code.visualstudio.com/docs/languages/cpp)



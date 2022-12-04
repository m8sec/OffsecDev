<p align="center">
  <img src="../_resources/img/0001.png">
</p>

# C++

### Table of Contents
  * [Examples](#examples)

## Examples
| File                                                   | Description                                                                                                                                                                                                                                                                                                                                                                                              |
|--------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| [suspended_injection.cpp](src/suspended_injection.cpp)          | Inject shellcode in remote process in a suspended state with protection ```PAGE_NOACCESS```. Sleeps to avoid runtime detections before modifying protections and calling ```ResumeThread``` - *Based on research by [FSecureLabs](https://labs.f-secure.com/blog/bypassing-windows-defender-runtime-scanning/) & [plackyhacker's PoC (C#)](https://github.com/plackyhacker/Suspended-Thread-Injection)*. |




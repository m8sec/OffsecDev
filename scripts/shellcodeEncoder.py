#!/usr/bin/env python3
'''
    Author: m8r0wn

    XOR shellcode for use in various inj techniques

    References:
        https://github.com/Arno0x/ShellcodeWrapper

    Usage:
        msfvenom -p /windows/x64/exec CMD="calc.exe" -f raw -o shellcode.bin
        python3 shellcodeEncoder.py shellcode.bin <key>
'''
import sys

def xor(data, key):
    l = len(key)
    keyAsInt = [x for x in map(ord, key)]
    return bytes(bytearray(((data[i] ^ keyAsInt[i % l]) for i in range(0,len(data)))))

def scFormat(data, scType='cpp'):
    # Format shellcode in various coding format (W.I.P)
    if scType == 'cpp':
        var  = 'char buf[] = "'
        pre  = '"'
        body = '\\x{:02x}'
        post = '"\n'
        end  = '";\n'
        
    # Print shellcode
    count = 0
    resp = var
    for x in data:
        if count >= 15:
            count = 0
            resp += post
            resp += pre
        resp += body.format(x)
        count += 1
    resp += end
    return resp

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("[!] Usage:")
        print("\tmsfvenom -p /windows/x64/exec CMD=\"calc.exe\" -f raw -o shellcode.bin")
        print("\tpython3 shellcodeEncoder.py shellcode.bin <key>\n")
        sys.exit(0)

    filename = sys.argv[1]
    key = sys.argv[2]

    with open(filename, 'rb') as f:
        data = f.read()

    xored_sc = xor(data, key)
    print(scFormat(xored_sc))

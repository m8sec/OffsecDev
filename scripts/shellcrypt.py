#!/usr/bin/env python3

'''
Author: m8r0wn
Shellcode obfuscation / encryption script

Usage:
msfvenom -p /windows/x64/exec CMD="calc.exe" -f raw -o shellcode.bin
python3 shellcrypt.py calc.bin -f c -e xor

Requirements
    pycryptodome

References:
    https://github.com/Arno0x/ShellcodeWrapper
    Sektor7 RTO
'''

import os
import sys
import hashlib
import argparse
from random import choice
from base64 import b64encode
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
from string import ascii_letters, digits
from Crypto.Random import get_random_bytes


#################################
# Shellcode Encryption
#################################
def xorEncrypt(data, key):
    l = len(key)
    keyAsInt = [x for x in map(ord, key)]
    return bytes(bytearray(((data[i] ^ keyAsInt[i % l]) for i in range(0, len(data)))))


def aesEncrypt(data, key):
    iv = 16 * b'\x00'
    cipher = AES.new(hashlib.sha256(key).digest(), AES.MODE_CBC, iv)
    return cipher.encrypt(pad(data, AES.block_size))


#################################
# Shellcode Formatting / Output
#################################
def scFormatPy(data, scVar='buf', wrap=False):
    return 0


def scFormat(data, scFormat='c', scVar='buf'):
    buf = ''
    if scFormat == 'c':
        buf += 'unsigned char {0}[] = {{ 0x'.format(scVar)
        buf += ', 0x'.join(hex(x)[2:] for x in data)
        buf += ' };'
    elif scFormat == 'py':
        buf += '{0} = "\\x'.format(scVar)
        buf += '\\x'.join(hex(x)[2:] for x in data)
        buf += '"'
    elif scFormat == 'csharp' or scFormat == 'cs':
        buf += 'byte[] {0} = new byte[{1}] {{ 0x'.format(scVar, len(data))
        buf += ', 0x'.join(hex(x)[2:] for x in data)
        buf += ' };'
    elif scFormat == 'base64':
        buf += '{0} = \"{1}\"'.format(scVar, b64encode(data).decode())
    elif scFormat == 'txt':
        # Generic key output for xor encryption
        buf += '{0} = \"{1}\"'.format(scVar, data)
    return buf


#################################
# Helper Functions
#################################
def gen_random_string(length=16):
    return ''.join([choice(ascii_letters + digits) for x in range(length)])


#################################
# EntryPoint / Main
#################################
def main(args):
    if os.path.exists(args.file):
        with open(args.file, 'rb') as data:
            print('\n[*] Processing shellcode...')
            print('[*] Encryption: {}'.format(args.encrypt))
            print('[*] Format: {}'.format(args.format))

            if args.encrypt == 'xor':
                key = gen_random_string(16)
                shellcode = xorEncrypt(data.read(), key)

            else:
                # Default to AES ecnryption
                key = get_random_bytes(16)
                shellcode = aesEncrypt(data.read(), key)

        print('[+] Shellcrypt complete:\n')
        print(scFormat(shellcode, args.format) + '\n')
        print(scFormat(key, args.format if args.encrypt != 'xor' else 'txt', scVar='key') + '\n')
        return True

    print('[!] Input file does not exist, try again.')
    return False


if __name__ == '__main__':
    version = '0.0.2'
    args = argparse.ArgumentParser(description="\t\t{} -{}".format(sys.argv[0], version), usage=argparse.SUPPRESS)
    args.add_argument('-e', '-encrypt', dest='encrypt', default='aes', choices=['xor', 'aes'], help='Encryption Type.')
    args.add_argument('-f', '-format', dest='format', default='c', choices=['c', 'csharp', 'cs', 'py', 'base64'], help='Output format.')
    args.add_argument(dest='file', nargs='?', help='Input Shellcode file.')
    args = args.parse_args()
    main(args)

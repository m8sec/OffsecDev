#!/usr/bin/env python3
# Author: @m8sec
# Description: Decrypt AES encrypted bytecode files by pyinstaller.

import os
import sys
import zlib
import tinyaes

key = sys.argv[1]                # Pyinstaller encryption Key
encrypt_file = sys.argv[2]       # Encrypted file path
struct_file  = sys.argv[3]       # Path to *exe_extracted\struct.pyc
decrypt_file = "decrypted.pyc"   # .pyc output file
CRYPT_BLOCK_SIZE = 16

# Grab pyc headers from packaged struct.pyc file
with open(os.path.join(struct_file), 'rb') as head:
    pyc_header = head.read()[:16]

# Decrypt file and decompress
with open(encrypt_file, 'rb') as d:
    data = d.read()
    cipher = tinyaes.AES(key.encode(), data[:CRYPT_BLOCK_SIZE])
    decrypt = cipher.CTR_xcrypt_buffer(data[CRYPT_BLOCK_SIZE:])
    plaintext = zlib.decompress(decrypt)

    # Write to file
    a = open(decrypt_file, 'wb')
    a.write(pyc_header)
    a.write(plaintext)
    a.close()
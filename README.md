# Anti-malware scanning interface (AMSI) Patching
Another AMSI bypass, this one is written in C++.

I recently changed the code to something a little bit more reliable. Inject the DLL into the PowerShell process (e.g., refelctive DLL injection) and `AmsiScanBuffer()` will be patched.

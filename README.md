# Anti-malware scanning interface (AMSI) Patching (and Hooking)
Another AMSI bypass, this one is written in C++.

I recently changed the code (`amsi.cpp`) to something a little bit more reliable. Inject the DLL into the PowerShell process (e.g., reflective DLL injection) and `AmsiScanBuffer()` will be patched.

I have also added `amsihook.cpp` as another method in which AMSI bypass can be achieved via DLL injection. This method hooks the `AmsiScanBuffer()` function and always returns `AMSI_RESULT_CLEAN`.

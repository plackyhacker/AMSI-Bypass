@ECHO OFF

cl.exe /nologo /Ox /MT /W0 /GS- /DNDEBUG /Tc *.cpp /link /OUT:amsi_bypass.exe /SUBSYSTEM:CONSOLE /MACHINE:x64

del *.obj

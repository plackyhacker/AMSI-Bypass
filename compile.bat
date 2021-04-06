@ECHO OFF

cl.exe /W0 /D_USRDLL /D_WINDLL amsi.cpp /MT /link /DLL /OUT:amsi-bypass.dll
echo Cleaning up...
del *.obj *.lib *.exp

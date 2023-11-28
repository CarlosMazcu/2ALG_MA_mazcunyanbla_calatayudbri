@ECHO off
cls
.\Premake5\Premake5.exe --file=premake_config.lua vs2019
FOR /D %%i IN (.\..\build\*.*) DO copy edk_memory_configuration.cfg .\%%i\vs2019\

@ECHO 8======D --- (.Y.)

pause

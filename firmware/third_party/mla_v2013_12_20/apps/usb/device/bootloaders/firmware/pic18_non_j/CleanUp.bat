@echo off
REM Remove files generated by compiler in this directory
REM and all subdirectories.

echo Removing *.d files...
del *.d /f /q /s
echo.

echo Removing *.dep files...
del *.dep /f /q /s
echo.

echo Removing *.p1 files...
del *.p1 /f /q /s
echo.

echo Removing *.pre files...
del *.pre /f /q /s
echo.

echo Removing *.as files...
del *.as /f /q /s
echo.

echo Removing funclist files...
del funclist /f /q /s
echo.

echo Removing *.hxl files...
del *.hxl /f /q /s
echo.

echo Removing *.$$$ files...
@del "%~dp0\*.$$$" /f /q /s 2>NUL
echo.

echo Removing *.bkx files...
@del "%~dp0\*.bkx" /f /q /s 2>NUL
echo.

echo Removing *.cce files...
@del "%~dp0\*.cce" /f /q /s 2>NUL
echo.

echo Removing *.cod files...
@del "%~dp0\*.cod" /f /q /s 2>NUL
echo.

echo Removing *.cof files...
@del "%~dp0\*.cof" /f /q /s 2>NUL
echo.

echo Removing *.err files...
del "%~dp0\*.err" /f /q /s 2>NUL
echo.

echo Removing *.hex files...
@del "%~dp0\*.hex" /f /q /s 2>NUL
echo.

echo Removing *.i files...
@del "%~dp0\*.i" /f /q /s 2>NUL
echo.

echo Removing *.lde files...
@del "%~dp0\*.lde" /f /q /s 2>NUL
echo.

echo Removing *.lst files...
@del "%~dp0\*.lst" /f /q /s 2>NUL
echo.

echo Removing *.obj files...
@del "%~dp0\*.obj" /f /q /s 2>NUL
echo.

echo Removing *.o files...
@del "%~dp0\*.o" /f /q /s 2>NUL
echo.

echo Removing *.rlf files...
@del "%~dp0\*.rlf" /f /q /s 2>NUL
echo.

echo Removing *.sym files...
@del "%~dp0\*.sym" /f /q /s 2>NUL
echo.

echo Removing *.sdb files...
@del "%~dp0\*.sdb" /f /q /s 2>NUL
echo.

echo Removing *.wat files...
@del "%~dp0\*.wat" /f /q /s 2>NUL
echo.

echo Removing *.mcs files...
@del "%~dp0\*.mcs" /f /q /s 2>NUL
echo.

echo Removing *.mptags files...
@del "%~dp0\*.mptags" /f /q /s 2>NUL
echo.

echo Removing *.tagsrc files...
@del "%~dp0\*.tagsrc" /f /q /s 2>NUL
echo.

echo Removing *.map files...
@del "%~dp0\*.map" /f /q /s 2>NUL
echo.

echo Removing *.elf files...
@del "%~dp0\*.elf" /f /q /s 2>NUL
echo.

echo Removing *.d files...
@del "%~dp0\*.d" /f /q /s 2>NUL
echo.

echo Removing MPLABX and MPLAB8 Compiler Output directories...
@rd "%~dp0\Objects" /S /Q 2>NUL
@rd "%~dp0\MPLAB.X\Build" /S /Q 2>NUL
@rd "%~dp0\MPLAB.X\dist" /S /Q 2>NUL
@rd "%~dp0\MPLAB.X\disassembly" /S /Q 2>NUL

echo Removing Makefile-genesis.properties files...
del Makefile-genesis.properties /f /q /s 2>NUL
echo.

echo Removing *.mk files...
del *.mk /f /q /s 2>NUL
echo.

echo Removing *.cmf files...
del *.cmf /f /q /s
echo.

echo Removing funclist. files...
del funclist /f /q /s 2>NUL
echo.

echo Removing *.bash files...
@del "%~dp0\*.bash" /f /q /s 2>NUL
echo.

echo Removing private.xml files...
@del "%~dp0\private.xml" /f /q /s 2>NUL
echo.


echo Done.
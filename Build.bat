@echo off

echo Building the RoyalFlush Editor...
echo:

call "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\RunUAT.bat" ^
BuildEditor -project=%CD%\RoyalFlush.uproject -notools

echo
pause
@echo off
set "PATH=%~dp0TDM-GCC-32\bin;%PATH%"
cd /D %1
"%~dp0TDM-GCC-32\bin\g++.exe" %2 -o %3
if exist %3 (
    echo Compilation Success
) else (
    echo Compilation Failed 1>&2
)
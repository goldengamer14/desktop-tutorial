@echo off
tasklist /fi "IMAGENAME eq cmd.exe" | find /i "cmd.exe" > nul
cls
if errorlevel 1 (
    echo CMD is not running.
    start cmd.exe /c
) else (
    echo CMD is already running.
)
if exist cmd_practice\ cd cmd_practice
cmd
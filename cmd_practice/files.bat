@echo off
cls
cmd /c
echo.
echo.
setlocal
echo Initializing log files...
setlocal enabledelayedexpansion
for %%f in (deletion_log.log log_robocopy.log my_git_repo_struct.txt) do (
    type nul > %%f
    echo Content %type %%f%
)
echo.
endlocal

echo Log files initialized.
echo.

echo Listing files in cmd_practice directory:
dir /b
echo.

@REM used >> instead of > to append logs
if exist ..\Maaz.js_copy (
    del /f /q /s ..\Maaz.js_copy\* >> deletion_log.log
    echo Files in Maaz.js_copy deleted.
) else (
    md ..\Maaz.js_copy
    echo Maaz.js_copy directory created.
)
echo.

robocopy /s ..\Maaz.js\ ..\Maaz.js_copy\ >> log_robocopy.log
if %errorlevel% neq 0 (
    echo Error occurred during file copy. Check log_robocopy.txt for details.
) else (
    echo Files copied successfully.
)

echo. >> deletion_log.log
echo. >> deletion_log.log
del /f /s /q ..\Maaz.js_copy\* >> deletion_log.log
rd /s /q ..\Maaz.js_copy
echo Maaz.js_copy cleaned up.

tree /f ..\ >> my_git_repo_struct.txt
if %errorlevel% neq 0 (
    echo Error occurred while generating directory structure.
) else (
    echo Directory structure saved to my_git_repo_struct.txt.
)
echo.

set "currentdir=%cd%"
echo You are currently working in ^<%currentdir%^>
endlocal
echo.
echo.
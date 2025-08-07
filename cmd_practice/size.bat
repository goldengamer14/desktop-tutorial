@echo off
echo.

call :get_file_size "deletion_log.log"
call :get_file_size "log_robocopy.log"
call :get_file_size "my_git_repo_struct.txt"
goto :eof


:get_file_size
setlocal enabledelayedexpansion
set "filepath=%~1"
@REM set "index=0"
@REM for /f "tokens=1,2,3" %%A in ('dir /-C "%filepath%" ^| findstr /R /C:"[0-9][0-9]*"') do (
@REM     set /a index+=1
@REM     if !index! equ 3 set "size=%%C"
@REM     echo A is %%A
@REM     echo B is %%B
@REM     echo C is %%C
@REM     echo.
@REM )
for %%A in ("%filepath%") do set "size=%%~zA"

set /a "KB_size=%size% / 1024"
set /a "MB_size=%KB_size% / 1024"
echo File size of "%filepath%" is !size! bytes, %KB_size% KB, or %MB_size% MB.
echo.

endlocal
goto :eof
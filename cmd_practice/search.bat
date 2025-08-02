@echo off
setlocal enabledelayedexpansion

call :SearchDirectory "." "string"
goto :eof @REM Go to end of script

:SearchDirectory
set "search_path=%~1"
set "search_name=%~2"

@REM echo Searching in: !search_path!
@REM echo Looking for directory: !search_name!

REM loop only through directories under search_path
for /d %%D in ("%search_path%\*") do (
    REM Check if the directory name matches
    echo %%~nxD | findstr /i /c:"%search_name%" >nul
    if not errorlevel 1 (
        echo Found: %%~fD
    ) else (
        call :SearchDirectory "%%~fD" "%search_name%"
    )
)
goto :eof
@REM exit /b

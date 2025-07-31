@echo off
if exist ".gitignore" (
    for /f "usebackq delims=" %%l in (".gitignore") do (
        if exist %%l (
            echo Deleting %%l
            set "var=%%l"
            setlocal enabledelayedexpansion
            set "var=!var:/=\!"
            echo !var!
            del /s /f /q "!var!"
            endlocal
        )
        if exist %%l (
            echo Deleting directory %%l
            set "var=%%l"
            setlocal enabledelayedexpansion
            set "var=!var:/=\!"
            echo !var!
            rmdir /s /q "!var!"
            endlocal
        )
        echo.
    )
) else (
    echo No .gitignore file found.
)
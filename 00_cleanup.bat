@echo off
if exist ".gitignore" (
    for /f "usebackq delims=" %%l in (".gitignore") do (
        setlocal enabledelayedexpansion
        set "var=%%l"
        if not "!var:*=%!"=="!var!" (
            set "var=!var:/=\!"
            del /s /f /q "!var!" 2 > nul
            echo Deleted all files matching %%l as !var!
            rd /s /q "!var!" 2 > nul
            echo Deleted directory matching %%l as !var!
        ) else (
            if exist %%l (
                echo Deleting %%l
                set "var=%%l"
                set "var=!var:/=\!"
                echo !var!
                del /s /f /q "!var!" 2 > nul
            )
            if exist %%l (
                echo Deleting directory %%l
                set "var=%%l"
                set "var=!var:/=\!"
                echo !var!
                rd /s /q "!var!" 2 > nul
            )
        )
        endlocal
        echo.
    )
) else (
    echo No .gitignore file found.
)
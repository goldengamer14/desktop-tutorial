@echo off
if exist ".gitignore" (
    for /f "usebackq delims=" %%l in (".gitignore") do (
        setlocal enabledelayedexpansion
        set "var=%%l"
        if not "!var:*=%!"=="!var!" (
            set "var=!var:/=\!"
            del /s /f /q "!var!"
            echo Deleted all files matching %%l as !var!
            rd /s /q "!var!"
            echo Deleted directory matching %%l as !var!
        )
        endlocal
        else (
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
        @REM endlocal
        )
        echo.
    )
) else (
    echo No .gitignore file found.
)
@REM Turn off echo or it'll print every command
@echo off

@REM Setting and using variables in a batch file
set /p name=Enter your name:
set city=Shegaon
echo Your name is %name% and you are from %city%

echo.
echo.
echo all variables being listed here:
set
echo.
echo.

@REM Creating local variables:
setlocal
set tempvar=349
echo The temporary variable %tempvar%
endlocal

@REM For loop variables:
@REM %%f, %%~nf, %%~xf, %%~dpF, %%~dpf, %%~dpnf, and other combinations:
for %%f in (*.*) do (
    echo file = "%%~nf" extension = "%%~xf" drive & ^path = "%%~dpF"
    echo file full path = "%%~dpf" ^path ^& file name = "%%~dpnf"
    echo ^path and file name with extension = "%%~dpnxf"
    echo.)

@REM Ranged For loop:
echo Numbers from 0 to 100 in steps of 5:
for /l %%i in (0, 5, 100) do echo %%i
echo.

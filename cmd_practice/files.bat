setlocal enabledelayedexpansion
md ..\Maaz.js_copy
robocopy /s ..\Maaz.js ..\Maaz.js_copy > log_robocopy.log
if %errorlevel% neq 0 
    echo Error occurred during file copy. Check log_robocopy.txt for details.
else 
    echo Files copied successfully.

del /s ..\Maaz.js_copy\*
rd ..\Maaz.js_copy
echo Maaz.js_copy cleaned up.

tree /f ..\ > my_git_repo_struct.txt
if %errorlevel% neq 0 
    echo Error occurred while generating directory structure.
else 
    echo Directory structure saved to my_git_repo_struct.txt.

set "currentdir=%cd%"
echo You are currently working in <%currentdir%>
endlocal
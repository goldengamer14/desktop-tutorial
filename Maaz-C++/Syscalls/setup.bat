set /p currfile=
set /p currdir=
set currdir=%cd%\%currdir%
set compile=gcc "%currdir%\%currfile%.c" -o "%currdir%\%currfile%.exe"
set "run=%compile% && "%currdir%\%currfile%.exe"

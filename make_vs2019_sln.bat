@echo off
set "CMAKE=cmake"
set "WORKING_DIR=%~dp0%"
set "SLN_DIR=%WORKING_DIR%\sln"
if not exist %SLN_DIR% md %SLN_DIR%
cd %SLN_DIR%
%CMAKE% %WORKING_DIR% -G "Visual Studio 16 2019" -A x64
if %ERRORLEVEL% NEQ 0 pause
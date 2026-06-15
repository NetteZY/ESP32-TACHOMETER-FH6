@echo off
cd /d "%~dp0"
title SimDash Configurator Windows Builder
echo ===================================================
echo   SimDash Configurator Standalone Executable Builder
echo ===================================================
echo.
echo [1/2] Installing package dependencies...
call npm install
echo.
echo [2/2] Compiling standalone Windows .exe...
call npm run dist -- --win
echo.
echo ===================================================
echo   Build complete! You can find the portable .exe in:
echo   configurator-app/dist/
echo ===================================================
echo.
pause

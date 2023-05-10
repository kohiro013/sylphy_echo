@echo off
if not exist %~dp0\build (
    echo Not Found build directory.
    pause >nul
)
for /f %%i in ('dir /A-D /B %~dp0\build\*.hex ^| find /c /v ""') do (
    set HEX_COUNT=%%i
)
if %HEX_COUNT%==1 (
    for %%i in ("%~dp0\build\*.hex") do (
        STM32_Programmer_CLI -c port=SWD -w  %%i -v -s
    )
) else (
    echo Too Many HEX file.
    pause >nul
)
rem pause >nul
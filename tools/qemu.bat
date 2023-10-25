@echo off

set dir=%1
if "%dir%" == "" (
    set dir=%~dp0
)

echo Running in %dir%

qemu-system-x86_64 -m 128M -drive file=fat:rw:%dir%,format=raw,media=disk -drive if=pflash,format=raw,file="%PROGRAMFILES%\qemu\share\edk2-x86_64-code.fd"

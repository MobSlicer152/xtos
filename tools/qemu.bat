@echo off

if "%1" == "" (
    echo Need disk image
    exit /b    
)

qemu-system-x86_64 -m 128M -drive file=fat:rw:%1,format=raw,media=disk -drive if=pflash,format=raw,file="%PROGRAMFILES%\qemu\share\edk2-x86_64-code.fd"

#!/usr/bin/env sh

if [ $# -lt 1 ]; then
    echo Need disk image
    exit
fi

qemu-system-x86_64 -m 128M -drive file=$1,format=raw,media=disk -drive if=pflash,format=raw,file="/usr/share/edk2/x64/OVMF.fd"

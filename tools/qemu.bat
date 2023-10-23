#!/usr/bin/env sh

if [ $# -gt 0 ]; then
    dir=$1
else
    dir=$(dirname $0)
fi

echo Running in $dir

qemu-system-x86_64 -drive file=fat:rw:$dir,format=raw,media=disk -drive if=pflash,format=raw,file="/usr/share/edk2/x64/OVMF.fd"

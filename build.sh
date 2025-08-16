#!/bin/bash
export PACKAGES_PATH=$PWD:$PWD/edk2
xxd -i assets/rocket.bmp > src/UEFI_fireworks/rocket.c
if [ "$1" == "debug" ]; then
    build -a X64 -t GCC5 -p UEFI_fireworks.dsc -b DEBUG
else
    build -a X64 -t GCC5 -p UEFI_fireworks.dsc
fi
mkdir -p build/EFI/BOOT/
cp edk2/Build/UEFI_fireworks/DEBUG_GCC5/X64/UEFI_fireworks.efi build/
cp edk2/Build/UEFI_fireworks/DEBUG_GCC5/X64/UEFI_fireworks.efi build/EFI/BOOT/BOOTX64.EFI

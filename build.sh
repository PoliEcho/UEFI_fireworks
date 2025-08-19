#!/bin/bash
export PACKAGES_PATH=$PWD:$PWD/edk2
magick assets/rocket_orig.bmp -type TrueColor -define  bmp:format=bmp3 -compress None assets/rocket.bmp
xxd -i assets/rocket.bmp > src/UEFI_fireworks/rocket.c
if [ "$1" == "debug" ]; then
    build -a X64 -t GCC5 -p UEFI_fireworks.dsc -b DEBUG
else
    build -a X64 -t GCC5 -p UEFI_fireworks.dsc -b RELEASE
fi
mkdir -p build/EFI/BOOT/
if [ "$1" == "debug" ]; then
cp $(find edk2/Build/UEFI_fireworks/DEBUG_GCC5/ -name "UEFI_fireworks.efi" -type f | head -1) build/
cp $(find edk2/Build/UEFI_fireworks/DEBUG_GCC5/ -name "UEFI_fireworks.efi" -type f | head -1) build/EFI/BOOT/BOOTX64.EFI
else
cp $(find edk2/Build/UEFI_fireworks/RELEASE_GCC5/ -name "UEFI_fireworks.efi" -type f | head -1) build/
cp $(find edk2/Build/UEFI_fireworks/RELEASE_GCC5/ -name "UEFI_fireworks.efi" -type f | head -1) build/EFI/BOOT/BOOTX64.EFI
fi
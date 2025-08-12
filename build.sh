#!/bin/bash
export PACKAGES_PATH=$PWD:$PWD/edk2
build -a X64 -t GCC5 -p UEFI_fireworks.dsc
mkdir -p build
cp edk2/Build/UEFI_fireworks/DEBUG_GCC5/X64/UEFI_fireworks.efi build/
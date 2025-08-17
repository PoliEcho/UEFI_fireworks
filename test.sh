#!/bin/bash

if [ $# -ge 1 ]; then
  OVMF_CODE_PATH=$1
else 
  OVMF_CODE_PATH=/usr/share/edk2/x64/OVMF_CODE.4m.fd
fi

qemu-system-x86_64 \
  -machine type=q35,accel=kvm \
  -drive if=pflash,format=raw,readonly=on,file="$OVMF_CODE_PATH" \
  -hda fat:rw:build \
  -boot order=c \
  -smp 4 \
  -s \
  -serial mon:stdio
#!/bin/bash

cp /usr/share/edk2/x64/OVMF_VARS.4m.fd ./my_ovmf_vars.fd

qemu-system-x86_64 \
  -machine type=q35,accel=kvm \
  -drive if=pflash,format=raw,readonly=on,file=/usr/share/edk2/x64/OVMF_CODE.4m.fd \
  -drive if=pflash,format=raw,file=./my_ovmf_vars.fd \
  -hda fat:rw:build \
  -boot order=c \
  -smp 4 \
  -s \
  -serial mon:stdio


rm ./my_ovmf_vars.fd

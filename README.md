# UEFI Fireworks
### Retro fireworks you can run on your modern UEFI system
Simple automatic fireworks app that runs on your UEFI firmware with basic controls.  
Primary purpose of this app is just to look at something nice.

> [!CAUTION]
> This app may cause epileptic seizures in people with epilepsy.  
> $\color{red}{\text{!!!If you have a history of seizures, do not use this program!!!}}$  
> I don't know if colorful expanding circles can cause seizures and i don't want for anyone to get hurt

## How to build

> [!IMPORTANT]  
> You will need these programs to compile:  
> - git  
> - gcc  
> - xxd  
> - ImageMagick (if you changed the firework image)

```bash
git submodule update --init --recursive
cd edk2
source edksetup.sh
make -C BaseTools
cd ..
./build.sh # to build for a different arch than X64 change TARGET_ARCH in build.sh
```

## How to use

### Download release

#### On real hardware

If you have an amd64 (x86_64) UEFI computer,  
download the release and copy UEFI_fireworks.efi to /EFI/BOOT/bootx64.efi (case-insensitive).

#### On VM

You will need OVMF firmware; you can get it here: https://qemu.weilnetz.de/test/ovmf/usr/share/OVMF/OVMF_CODE_4M.fd

Run:
```bash
qemu-system-x86_64 -machine type=q35,accel=kvm -drive if=pflash,format=raw,readonly=on,file="$OVMF_CODE_PATH" -hda fat:rw:build -boot order=c -smp 4 -s -serial mon:stdio
```

### Build

#### On VM

You will need OVMF firmware; you can get it here: https://qemu.weilnetz.de/test/ovmf/usr/share/OVMF/OVMF_CODE_4M.fd

Run:
```bash
./test.sh $OVMF_CODE_PATH  # only works on AMD64
```
to run in a VM using QEMU.

#### On real hardware

Copy:
```
edk2/Build/UEFI_fireworks/DEBUG_GCC5//UEFI_fireworks.efi
```
to a FAT32 or FAT16 USB at the following location:

| Architecture    | Default Boot Path        |
| --------------- | ------------------------ |
| x86_64 (AMD64)  | `/EFI/BOOT/BOOTX64.EFI`  |
| x86 (IA32)      | `/EFI/BOOT/BOOTIA32.EFI` |
| ARM64 (AARCH64) | `/EFI/BOOT/BOOTAA64.EFI` |
| ARM (32-bit)    | `/EFI/BOOT/BOOTARM.EFI`  |

## Controlling

| Key        | Action       | Note                               |
| ---------- | ------------ | ---------------------------------- |
| ARROW_UP   | speed up     | delays under 1ms may be unreliable |
| ARROW_DOWN | slow down    | max delay is about UINT32_MAX μs   |
| PAGE_UP    | speed up     | 10× step                           |
| PAGE_DOWN  | slow down    | 10× step                           |
| Home       | reset speed  | reset frame delay to 10ms          |
| Delete     | clear screen |                                    |

## Showcase
In QEMU/KVM

https://github.com/user-attachments/assets/275e28aa-eb2e-4348-ac08-95f909c46a1f

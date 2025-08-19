
# UEFI Fireworks  

## How to build  

```bash
git submodule update --init --recursive
cd  edk2
source edksetup.sh
make -C BaseTools
cd ..
./build.sh
```

## How to use  

### Download release  

#### On Real hardware  

if your have amd64(x86_64) UEFI computer  
Download release and copy the UEFI_fireworks.efi to <USB_DRIVE>/EFI/BOOT/bootx64.efi case independent  

#### On VM  

run:  
you will need OVMF firmware you can get it [here](https://qemu.weilnetz.de/test/ovmf/usr/share/OVMF/OVMF_CODE_4M.fd)
```bash
qemu-system-x86_64 -machine type=q35,accel=kvm -drive if=pflash,format=raw,readonly=on,file="$OVMF_CODE_PATH" -hda fat:rw:build -boot order=c -smp 4 -s -serial mon:stdio
```  

### Build  

#### On VM  

you will need OVMF firmware you can get it [here](https://qemu.weilnetz.de/test/ovmf/usr/share/OVMF/OVMF_CODE_4M.fd)  
run ```./test.sh $OVMF_CODE_PATH# only works on AMD64``` to run in VM using QEMU  

#### On Real hardware  

copy ```edk2/Build/UEFI_fireworks/DEBUG_GCC5/<YOUR ARCH>/UEFI_fireworks.efi``` to FAT32 or FAT16 USB at location:

| Architecture        | Default Boot Path        |
| ------------------- | ------------------------ |
| **x86_64 (AMD64)**  | `/EFI/BOOT/BOOTX64.EFI`  |
| **x86 (IA32)**      | `/EFI/BOOT/BOOTIA32.EFI` |
| **ARM64 (AArch64)** | `/EFI/BOOT/BOOTAA64.EFI` |
| **ARM (32-bit)**    | `/EFI/BOOT/BOOTARM.EFI`  |

## Controling  

| Key        | Action       | Note                                |
| ---------- | ------------ | ----------------------------------- |
| ARROW_UP   | speed up     | delays under 1ms may be unrelayable |
| ARROW_DOWN | slow down    | max delay is about UINT32_MAXμs     |
| PAGE_UP    | speed up     | 10x step                            |
| PAGE_DOWN  | speed down   | 10x step                            |
| Home key   | reset speed  | reset frame delay to 10ms           |
| Delete key | clear screen |                                     |

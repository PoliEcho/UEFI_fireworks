
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

### On VM  

run ```./test.sh``` to run in VM using QEMU  

### On Real hardware  

copy ```edk2/Build/UEFI_fireworks/DEBUG_GCC5/<YOUR ARCH>/UEFI_fireworks.efi``` to FAT32 or FAT16 USB at location:

| Architecture | Default Boot Path |
|--------------|-------------------|
| **x86_64 (AMD64)** | `\EFI\BOOT\BOOTX64.EFI` |
| **x86 (IA32)** | `\EFI\BOOT\BOOTIA32.EFI` |
| **ARM64 (AArch64)** | `\EFI\BOOT\BOOTAA64.EFI` |
| **ARM (32-bit)** | `\EFI\BOOT\BOOTARM.EFI` |

## Controling  

| Key | Action | Note |
|-----|--------|------|
| ARROW_UP | speed up | delays under 1ms may be unrelayable |
| ARROW_DOWN | slow down | max delay is about UINT32_MAXμs |
| PAGE_UP | speed up | 10x step |
| PAGE_DOWN | speed down | 10x step |
| Home key | reset speed | reset frame delay to 10ms |
| Delete key | clear screen |  |
